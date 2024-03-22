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
			Log(TEXT("Logger Initialized"));
			UE_LOG(LogTemp, Warning, TEXT("Logger Initialized with path: %s"), *Path);
		}
	}
}

void UCLogger::ShutDown()
{
	if(LogFile)
	{
		Log("Shutting Down");
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
	}
}

