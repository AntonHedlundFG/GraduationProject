#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EVictoryConditions : uint8
{
	EVC_None		UMETA(DisplayName = "None"),
	EVC_KillEnemies	UMETA(DisplayName = "KillEnemies"),
	EVC_PickUpKeys	UMETA(DisplayName = "PickUpKeys"),
	EVC_MAX			UMETA(Hidden)
};

inline FString ToString(EVictoryConditions Slot)
{
	switch(Slot)
	{
	case EVictoryConditions::EVC_None:
		return "None";
	case EVictoryConditions::EVC_KillEnemies:
		return "Kill Enemies";
	case EVictoryConditions::EVC_PickUpKeys:
		return "Pick Up Keys";
	default:
		return "Invalid";
	}
}