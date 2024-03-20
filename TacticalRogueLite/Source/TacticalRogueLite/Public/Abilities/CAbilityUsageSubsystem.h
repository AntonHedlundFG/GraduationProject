// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CAbilityUsageSubsystem.generated.h"

class ACGameState;
enum class EItemSlots : uint8;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAbilityUsageSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UCAbilityUsageSubsystem, STATGROUP_Tickables); }

	void InitiateAbilityUse(EItemSlots ItemSlot);
	void CancelAbilityUse();
	void UndoAbility();

private:

	ACGameState* GetGameState();
	ACGameState* GameStateRef;
	
};
