#pragma once

#include "CoreMinimal.h"
#include "CLogManager.generated.h"

#pragma region ELogCategory Enum

UENUM(BlueprintType)
enum class ELogCategory : uint8
{
	NONE			UMETA(Hidden),
	LC_Info			UMETA(DisplayName = "Info"),
	LC_Warning		UMETA(DisplayName = "Warning"),
	LC_Error		UMETA(DisplayName = "Error"),
	LC_Gameplay		UMETA(DisplayName = "Gameplay"),
	LC_Network		UMETA(DisplayName = "Network"),
	MAX				UMETA(Hidden)
};

inline FString ToString(ELogCategory LogCategory)
{
	switch (LogCategory)
	{
		case ELogCategory::LC_Info: return TEXT("[INFO]");
		case ELogCategory::LC_Warning: return TEXT("[WARNING]");
		case ELogCategory::LC_Error: return TEXT("[ERROR]");
		case ELogCategory::LC_Gameplay: return TEXT("[GAMEPLAY]");
		case ELogCategory::LC_Network: return TEXT("[NETWORK]");
		default: return TEXT("[UNKNOWN]");
	}
}

#pragma endregion

#pragma region Logging Macros

//Gameplay messages will always attempt to log to all clients and server. If called on a client
//the message only displays on the client.
#define LOG(Category, Message, ...) \
UCLogManager::Log(Category, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_INFO(Message, ...) \
UCLogManager::Log(ELogCategory::LC_Info, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_WARNING(Message, ...) \
UCLogManager::Log(ELogCategory::LC_Warning, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_ERROR(Message, ...) \
UCLogManager::Log(ELogCategory::LC_Error, FString::Printf(TEXT(Message), ##__VA_ARGS__))

//Gameplay messages will always attempt to log to all clients and server. If called on a client
//the message only displays on the client.
#define LOG_GAMEPLAY(Message, ...) \
UCLogManager::LogFromServer(ELogCategory::LC_Gameplay, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_NETWORK(Message, ...) \
UCLogManager::Log(ELogCategory::LC_Network, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewLogEntry, ELogCategory, Category, const FString&, Message);

// *
// FLogger wrapper for access through Blueprints and C++
// *
UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCLogManager : public UObject
{
	GENERATED_BODY()

public:
	// Log a message with a category
	UFUNCTION(Category = "Logging", meta = (DisplayName = "Log"))
	static void Log(const ELogCategory Category, const FString& Message);
	
	// Attempts to log a message on ALL clients and server. If called on a client
	// message is only logged locally on that client.
	static void LogFromServer(const ELogCategory Category, const FString& Message);

	// Read all log entries from the log file
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Read Log"))
	static TArray<FString> GetAllLogEntries();

	// Read all log entries from the selected category from the log file 
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Read Log By Category"))
	static TArray<FString> GetAllLogEntriesByCategory(const ELogCategory Category);

	// Get a log by its ID
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Get Recent Log Entry"))
	static FString GetRecentLogEntry(int64 LogID);

	// Rotate the log file and start writing to a new one
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Rotate LogFile"))
	static void RotateLogFile();
	
	// Get the singleton instance of the logger
	UFUNCTION(BlueprintPure, Category = "Logging", meta = (DisplayName = "Get LogManager Instance"))
	static UCLogManager* Get();

	// Start and stop the logger
	static void StartUp();
	static void ShutDown();

	// Delegate for broadcasting log events
	UPROPERTY(BlueprintAssignable, Category = "Logging")
	FOnNewLogEntry OnNewLogEntry;

	// Delegate for managing server-wide broadcasts. The GameState listens to this event.
	UPROPERTY(BlueprintAssignable, Category = "Logging")
	FOnNewLogEntry OnServerBroadcastMessage;

private:
	static UCLogManager* Instance;
};

UCLASS()
class TACTICALROGUELITE_API UCLogManagerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Blueprint-accessible logging function
	UFUNCTION( BlueprintCallable, Category = "Logging", meta = (DisplayName = "Log") )
	static void Log(const ELogCategory Category, const FString& Message)
	{
		UCLogManager::Log(Category, Message);
	}

};
