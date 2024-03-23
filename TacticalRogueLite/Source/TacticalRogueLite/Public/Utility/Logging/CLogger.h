#pragma once

#include "CoreMinimal.h"
#include "CLogger.generated.h"

#pragma region ELogCategory Enum

UENUM(BlueprintType)
enum class ELogCategory : uint8
{
	LC_INFO			UMETA(DisplayName = "Info"),
	LC_WARNING		UMETA(DisplayName = "Warning"),
	LC_ERROR		UMETA(DisplayName = "Error"),
	LC_GAMEPLAY		UMETA(DisplayName = "Gameplay"),
	LC_NETWORK		UMETA(DisplayName = "Network"),
	MAX				UMETA(Hidden)
};

inline FString ToString(ELogCategory LogCategory)
{
	switch (LogCategory)
	{
		case ELogCategory::LC_INFO: return TEXT("[INFO]");
		case ELogCategory::LC_WARNING: return TEXT("[WARNING]");
		case ELogCategory::LC_ERROR: return TEXT("[ERROR]");
		case ELogCategory::LC_GAMEPLAY: return TEXT("[GAMEPLAY]");
		case ELogCategory::LC_NETWORK: return TEXT("[NETWORK]");
		default: return TEXT("[UNKNOWN]");
	}
}

#pragma endregion

#pragma region Logging Macros

#define LOG(Category, Message, ...) \
UCLogger::Log(Category, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_INFO(Message, ...) \
UCLogger::Log(ELogCategory::LC_INFO, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_WARNING(Message, ...) \
UCLogger::Log(ELogCategory::LC_WARNING, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_ERROR(Message, ...) \
UCLogger::Log(ELogCategory::LC_ERROR, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_GAMEPLAY(Message, ...) \
UCLogger::Log(ELogCategory::LC_GAMEPLAY, FString::Printf(TEXT(Message), ##__VA_ARGS__))

#define LOG_NETWORK(Message, ...) \
UCLogger::Log(ELogCategory::LC_NETWORK, FString::Printf(TEXT(Message), ##__VA_ARGS__))

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
	// Log a message with a category
	UFUNCTION(BlueprintCallable, Category = "Logging")
	static void Log(ELogCategory Category = ELogCategory::LC_INFO, const FString& Message = TEXT(""));

	// Get the singleton instance of the logger
	UFUNCTION(BlueprintPure, Category = "Logging", meta = (DisplayName = "Get Logger Instance"))
	static UCLogger* Get();

	// Delegate for broadcasting log events
	UPROPERTY(BlueprintAssignable, Category = "Logging")
	FOnNewLogEntry OnNewLogEntry;

private:
	UCLogger() {}
	static UCLogger* Instance;
};
