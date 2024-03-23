#include "Utility/Logging/CLogger.h"
#include "Utility/Logging/FLogger.h"

UCLogger* UCLogger::Instance = nullptr;

UCLogger* UCLogger::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCLogger>();
		Instance->AddToRoot(); // Prevent Garbage Collection
	}
	return Instance;
}

void UCLogger::BlueprintLog(ELogCategory Category, const FString& Message)
{
	const FString LogMessage = FString::Printf(TEXT("[Blueprint] %s"), *Message);
	Log(Category, LogMessage);
}

void UCLogger::Log(ELogCategory Category, const FString& Message)
{
	// Convert the enum to string and prepend to the message
	const FString LogMessage = FString::Printf(TEXT("%s %s"),*ToString(Category), *Message);

	// Use FLogger for actual logging
	FLogger::Get().Log(LogMessage);

	// Broadcast if Instance exists
	if (Instance) 
	{
		Instance->OnNewLogEntry.Broadcast(Category, Message);
	}

	// Log to Unreal
	switch (Category)
	{
		case ELogCategory::LC_WARNING:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			break;
		case ELogCategory::LC_ERROR:
			UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	}
	
}

TArray<FString> UCLogger::ReadLog()
{
	return FLogger::Get().ReadLog();
}

TArray<FString> UCLogger::ReadLogByCategory(ELogCategory Category)
{
	TArray<FString> LogEntries = FLogger::Get().ReadLog();
	TArray<FString> FilteredEntries;

	for (FString LogEntry : LogEntries)
	{
		FString CategoryString = ToString(Category);
		if (LogEntry.Contains(CategoryString))
		{
			FilteredEntries.Add(LogEntry);
		}
	}
	
	return FilteredEntries;
}
