// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCombatLogger.generated.h"

UENUM(BlueprintType)
enum class ECombatLogCategory : uint8
{
	DEFAULT			UMETA(DisplayName = "Default"),
	COMBAT			UMETA(DisplayName = "Combat"),
	RED				UMETA(DisplayName = "Red"),
	ORANGE			UMETA(DisplayName = "Orange"),
	CYAN			UMETA(DisplayName = "Cyan"),
	GREEN			UMETA(DisplayName = "Green")
};

inline FString ToCombatLogStyleBegin(ECombatLogCategory LogCategory)
{
	switch (LogCategory)
	{
	case ECombatLogCategory::COMBAT: return TEXT("<Combat>");
	case ECombatLogCategory::RED: return TEXT("<Red>");
	case ECombatLogCategory::ORANGE: return TEXT("<Orange>");
	case ECombatLogCategory::CYAN: return TEXT("<Cyan>");
	case ECombatLogCategory::GREEN: return TEXT("<Green>");
	default: return TEXT("");
	}
}
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCCombatLogger : public UObject
{
	GENERATED_BODY()
public:
	inline static const FString StyleEnd = "</>";
	UFUNCTION(BlueprintCallable, Category = "CombatLogging")
	static FString Format(ECombatLogCategory Category,FString Message);
	UFUNCTION(BlueprintCallable, Category = "CombatLogging")
	static FString FormatIT(ECombatLogCategory Category, FString Instigator, FString Target,FString Message);
	UFUNCTION(BlueprintCallable, Category = "CombatLogging")
	static void Log(FString Message, ECombatLogCategory Category);
};
