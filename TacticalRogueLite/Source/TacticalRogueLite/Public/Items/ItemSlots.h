// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EItemSlots : uint8
{
	EIS_NONE,
	EIS_Boots	UMETA(DisplayName = "Boots"),
	EIS_Weapon	UMETA(DisplayName = "Weapon"),
	EIS_MAX
};
