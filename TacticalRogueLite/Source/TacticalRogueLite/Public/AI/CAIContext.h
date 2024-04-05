#pragma once

#include "CoreMinimal.h"
#include "CAIContext.generated.h"

class ACUnit;

USTRUCT(BlueprintType)
struct FCAIContext 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<ACUnit*> AIUnits;
	UPROPERTY(BlueprintReadOnly)
	TArray<ACUnit*> PlayerUnits;
	UPROPERTY(BlueprintReadOnly)
	ACUnit* CurrentUnit;
	// UPROPERTY(BlueprintReadOnly)
	// struct FAbility CurrentAbility;
	
};
