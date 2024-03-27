// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemSlots : uint8
{
	EIS_None	UMETA(DisplayName = "None"),
	EIS_Boots	UMETA(DisplayName = "Boots"),
	EIS_Weapon	UMETA(DisplayName = "Weapon"),
	EIS_Body	UMETA(DisplayName = "Body Armor"),
	EIS_Helmet	UMETA(DisplayName = "Helmet"),
	EIS_Ring	UMETA(DisplayName = "Ring"),
	EIS_MAX		UMETA(Hidden)
};

inline FString ToString(EItemSlots Slot)
{
	switch(Slot)
	{
	case EItemSlots::EIS_None:
		return "None";
	case EItemSlots::EIS_Boots:
		return "Boots";
	case EItemSlots::EIS_Weapon:
		return "Weapon";
	case EItemSlots::EIS_Body:
		return "Body Armor";
	case EItemSlots::EIS_Helmet:
		return "Helmet";
	case EItemSlots::EIS_Ring:
		return "Ring";
	default:
		return "Invalid";
	}
}