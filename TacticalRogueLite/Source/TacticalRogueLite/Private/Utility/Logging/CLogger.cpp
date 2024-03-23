#include "Utility/Logging/CLogger.h"

UCLogger* UCLogger::Instance = nullptr;

UCLogger::UCLogger()
{
	Initialize();
}

UCLogger::~UCLogger()
{
	ShutDown();
}

UCLogger* UCLogger::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCLogger>();
		Instance->AddToRoot(); // Prevent Garbage Collection
	}
	return Instance;
}

void UCLogger::Initialize()
{
	if(!bIsInitialized)
	{
		const FString Path = FPaths::ProjectLogDir() + TEXT("TacticalRogueLiteLogs/") + FDateTime::Now().ToString() + TEXT(".log");
		LogFile = IFileManager::Get().CreateFileWriter(*Path);
		if(LogFile)
		{
			bIsInitialized = true;
			LOG_INFO("Logger Initialized");
		}
	}
}

void UCLogger::ShutDown()
{
	if(LogFile)
	{
		LOG_INFO("Logger Shutting Down");
		LogFile->Close();
		LogFile = nullptr;
		bIsInitialized = false;
	}
}

void UCLogger::Log(const FString Message)
{
	if(LogFile)
	{
		FString LogMessage = FDateTime::Now().ToString() + TEXT(" - ") + Message + LINE_TERMINATOR;
		LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
		LogFile->Flush();
		UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	}
}

