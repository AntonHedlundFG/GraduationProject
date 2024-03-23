#pragma once

#include "CoreMinimal.h"
#include "CLogger.generated.h"


enum class ELogCategory : uint8
{
	Info,
	Warning,
	Error,
	Gameplay,
	Network
};

inline FString ToString(ELogCategory LogCategory)
{
	switch (LogCategory)
	{
	case ELogCategory::Info: return TEXT("INFO");
	case ELogCategory::Warning: return TEXT("WARNING");
	case ELogCategory::Error: return TEXT("ERROR");
	case ELogCategory::Gameplay: return TEXT("GAMEPLAY");
	case ELogCategory::Network: return TEXT("NETWORK");
	default: return TEXT("UNKNOWN");
	}
}

#define LOG(Category, Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(Category), ##__VA_ARGS__))

#define LOG_INFO(Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(ELogCategory::Info), ##__VA_ARGS__))

#define LOG_WARNING(Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(ELogCategory::Warning), ##__VA_ARGS__))

#define LOG_ERROR(Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(ELogCategory::Error), ##__VA_ARGS__))

#define LOG_GAMEPLAY(Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(ELogCategory::Gameplay), ##__VA_ARGS__))

#define LOG_NETWORK(Message, ...) \
UCLogger::Get()->Log(FString::Printf(TEXT("%s: " Message), *ToString(ELogCategory::Network), ##__VA_ARGS__))

UCLASS(BlueprintType)
class UCLogger : public UObject
{
	GENERATED_BODY()
	UCLogger();
	virtual ~UCLogger() override;
	
public:
	static UCLogger* Get();

	void Initialize();
	void ShutDown();
	
	void Log(const FString Message);
	
private:
	static UCLogger* Instance;

	bool bIsInitialized = false;

	FArchive* LogFile;
};
