#include "Utility/Logging/CLogManager.h"
#include "Utility/Logging/LogManager.h"

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
	LogManager::Initialize();
}

void UCLogManager::ShutDown()
{
	LogManager::ShutDown();
}

void UCLogManager::Log(const ELogCategory Category, const FString& Message)
{
	
#if UE_BUILD_SHIPPING
	// Only log gameplay messages in shipping build.
	if(Category != ELogCategory::LC_Gameplay)
		return;
	
#endif
	
	// Convert the enum to string and prepend to the message
	const FString LogMessage = FString::Printf(TEXT("%s %s"), *ToString(Category), *Message);

	// Convert FString (UTF-16) to std::string (UTF-8)
	const std::string StdLogMessage(TCHAR_TO_UTF8(*LogMessage));
	
	// Use FLogger for actual logging
	LogManager::Log(StdLogMessage);

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

void UCLogManager::LogFromServer(const ELogCategory Category, const FString& Message)
{
	if (Get())
		Get()->OnServerBroadcastMessage.Broadcast(Category, Message);
}

TArray<FString> UCLogManager::GetAllLogEntries()
{
	LOG_INFO("Reading log entries");
	std::vector<std::string> LogMessages = LogManager::ReadLog();

	TArray<FString> LogEntries;
	for (std::string LogMessage : LogMessages)
	{
		LogEntries.Add(FString(LogMessage.c_str()));
	}
	
	return LogEntries;
}

TArray<FString> UCLogManager::GetAllLogEntriesByCategory(const ELogCategory Category)
{
	TArray<FString> LogEntries = GetAllLogEntries();
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
	FString LogEntry = FString(LogManager::GetRecentLogEntry(LogID).c_str());
	return LogEntry;
}

void UCLogManager::RotateLogFile()
{
	LogManager::RotateLogFile();
}
