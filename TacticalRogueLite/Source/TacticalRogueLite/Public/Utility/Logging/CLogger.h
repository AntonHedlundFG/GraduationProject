#pragma once

#include "CoreMinimal.h"
#include "CLogger.generated.h"

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

#define LOG(Category, Message, ...) \
UCLogger::Log(Category, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_INFO(Message, ...) \
UCLogger::Log(ELogCategory::LC_Info, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_WARNING(Message, ...) \
UCLogger::Log(ELogCategory::LC_Warning, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_ERROR(Message, ...) \
UCLogger::Log(ELogCategory::LC_Error, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_GAMEPLAY(Message, ...) \
UCLogger::Log(ELogCategory::LC_Gameplay, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_NETWORK(Message, ...) \
UCLogger::Log(ELogCategory::LC_Network, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewLogEntry, ELogCategory, Category, const FString&, Message);

// *
// FLogger wrapper for access through Blueprints and C++
// *
UCLASS(BlueprintType)
class UCLogger : public UObject
{
	GENERATED_BODY()

public:
	// Blueprint-accessible logging function
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Log"))
	static void BlueprintLog(ELogCategory Category = ELogCategory::LC_Info, const FString& Message = TEXT(""));

	// Log a message with a category
	static void Log(ELogCategory Category, const FString& Message);

	// Read all log entries from the log file
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Read Log"))
	static TArray<FString> ReadLog();

	// Read all log entries from the selected category from the log file 
	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Read Log By Category"))
	static TArray<FString> ReadLogByCategory(ELogCategory Category);

	UFUNCTION(BlueprintCallable, Category = "Logging", meta = (DisplayName = "Rotate LogFile"))
	static void RotateLogFile();
	
	// Get the singleton instance of the logger
	UFUNCTION(BlueprintPure, Category = "Logging", meta = (DisplayName = "Get Logger Instance"))
	static UCLogger* Get();

	// Start and stop the logger
	static void StartUp();
	static void ShutDown();

	// Delegate for broadcasting log events
	UPROPERTY(BlueprintAssignable, Category = "Logging")
	FOnNewLogEntry OnNewLogEntry;

private:
	static UCLogger* Instance;
};


