#include "Utility/Logging/FLogger.h"

FLogger& FLogger::Get()
{
	static FLogger Instance;
	return Instance;
}

FLogger::FLogger()
{
	Initialize();
}

FLogger::~FLogger()
{
	ShutDown();
}

void FLogger::Log(const FString& Message)
{
	// Lock the log and make other threads wait
	std::lock_guard Lock(LogMutex);
	// Log to file
	if(LogFile)
	{
		const FString LogMessage = FString::Printf(TEXT("%s - %s\n"), *FDateTime::Now().ToString(), *Message);
		LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
		LogFile->Flush();
	}
}

void FLogger::Initialize()
{
	// Create a new log file if it doesn't exist
	if(!LogFile)
	{
		const FString CompleteLogPath = FPaths::ProjectLogDir() + RelativeLogPath + FDateTime::Now().ToString() + TEXT(".log");
		LogFile = IFileManager::Get().CreateFileWriter(*CompleteLogPath);
		if(LogFile)
		{
			Log(FString::Printf(TEXT("%s - %s\n"), *LogInternalMessage, TEXT("Logger Initialized")));
		}
	}
}

void FLogger::ShutDown()
{
	// Close the log file
	if(LogFile)
	{
		Log(FString::Printf(TEXT("%s - %s\n"), *LogInternalMessage, TEXT("Logger Shut Down")));
		LogFile->Close();
		delete LogFile;
		LogFile = nullptr;
	}
}