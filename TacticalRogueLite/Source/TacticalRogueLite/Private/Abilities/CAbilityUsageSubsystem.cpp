// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CAbilityUsageSubsystem.h"
#include "Items/ItemSlots.h"
#include "CGameState.h"

void UCAbilityUsageSubsystem::InitiateAbilityUse(EItemSlots ItemSlot)
{
}

void UCAbilityUsageSubsystem::CancelAbilityUse()
{
}

void UCAbilityUsageSubsystem::UndoAbility()
{
}

ACGameState* UCAbilityUsageSubsystem::GetGameState()
{
	if (!GameStateRef)
	{
		GameStateRef = GetWorld()->GetGameState<ACGameState>();
	}
	return GameStateRef;
}
