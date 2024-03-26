#include "Utility/Logging/CLogManager.h"
#include "Utility/Logging/FLogManager.h"

UCLogManager* UCLogManager::Instance = nullptr;

UCLogManager* UCLogManager::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCLogManager>();
		Instance->AddToRoot(); // Prevent Garbage Collection
	}
	return Instance;
}

void UCLogManager::StartUp()
{
	FLogManager::Initialize();
}

void UCLogManager::ShutDown()
{
	FLogManager::ShutDown();
}


void UCLogManager::BlueprintLog(ELogCategory Category, const FString& Message)
{
	const FString LogMessage = FString::Printf(TEXT("[Blueprint] %s"), *Message);
	Log(Category, LogMessage);
}

void UCLogManager::Log(ELogCategory Category, const FString& Message)
{
	// Convert the enum to string and prepend to the message
	const FString LogMessage = FString::Printf(TEXT("%s %s"), *ToString(Category), *Message);

	// Use FLogger for actual logging
	FLogManager::Log(LogMessage);

	// Broadcast if Instance exists
	if (Get()) 
	{
		Instance->OnNewLogEntry.Broadcast(Category, Message);
	}

	// Log to Unreal
	switch (Category)
	{
		case ELogCategory::LC_Warning:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			break;
		case ELogCategory::LC_Error:
			UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	}
	
}

TArray<FString> UCLogManager::GetAllLogEntries()
{
	LOG_INFO("Reading log entries");
	return FLogManager::ReadLog();
}

TArray<FString> UCLogManager::GetAllLogEntriesByCategory(ELogCategory Category)
{
	TArray<FString> LogEntries = FLogManager::ReadLog();
	TArray<FString> FilteredEntries;

	for (FString LogEntry : LogEntries)
	{
		FString CategoryString = ToString(Category);
		if (LogEntry.Contains(CategoryString))
		{
			FilteredEntries.Add(LogEntry);
		}
	}

	LOG_INFO("Read %d log entries for category %s", FilteredEntries.Num(), *ToString(Category));
	
	return FilteredEntries;
}

FString UCLogManager::GetRecentLogEntry(int64 LogID)
{
	LOG_INFO("Getting recent log entry with ID: %lld", LogID);
	return FLogManager::GetRecentLogEntry(LogID);
}

void UCLogManager::RotateLogFile()
{
	FLogManager::RotateLogFile();
}
