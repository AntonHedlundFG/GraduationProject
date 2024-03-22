#pragma once

#include "CoreMinimal.h"
#include "CLogger.generated.h"

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
