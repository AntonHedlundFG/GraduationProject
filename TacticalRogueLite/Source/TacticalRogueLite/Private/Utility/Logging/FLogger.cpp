#include "Utility/Logging/FLogger.h"

FString FLogger::LogFilePath;

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
	// Ensure exclusive access while operating on the log
	std::lock_guard Lock(LogMutex);
	// Log to file
	if(LogFile)
	{
		const FString LogMessage = FString::Printf(TEXT("%s - %s\n"), *FDateTime::Now().ToString(), *Message);
		LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
		LogFile->Flush();
	}
}

TArray<FString> FLogger::ReadLog()
{
	TArray<FString> LogEntries;
	
	{
		// Ensure exclusive access while operating on the log
		std::lock_guard Lock(LogMutex);

		// Temporarily close the log file to read from it
		if(LogFile)
		{
			LogFile->Flush();
			LogFile->Close();
			delete LogFile;
			LogFile = nullptr;
		}

		// Read all log entries from the file
		FFileHelper::LoadFileToStringArray(LogEntries, *GetLogPath());
	}

	// Reopen the log file for writing
	LogFile = IFileManager::Get().CreateFileWriter(*GetLogPath(), FILEWRITE_Append);

	return LogEntries;
}

void FLogger::Initialize()
{
	// Open the log file for writing
	if(!LogFile)
	{
		LogFile = IFileManager::Get().CreateFileWriter(*GetLogPath());
		if(LogFile)
		{
			Log(FString::Printf(TEXT("%s - %s\n"), *LogInternalTag, TEXT("Logger Initialized")));
		}
	}
}

void FLogger::ShutDown()
{
	// Close the log file
	if(LogFile)
	{
		Log(FString::Printf(TEXT("%s - %s\n"), *LogInternalTag, TEXT("Logger Shut Down")));
		LogFile->Flush();
		LogFile->Close();
		delete LogFile;
		LogFile = nullptr;
	}
}
