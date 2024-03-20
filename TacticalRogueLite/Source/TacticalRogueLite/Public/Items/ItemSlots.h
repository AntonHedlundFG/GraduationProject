// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
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
