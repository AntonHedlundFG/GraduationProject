// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievements/CVictoryCondition.h"
#include "CGameState.h"
#include "CGameMode.h"
#include "Attributes\CAttributeComponent.h"

void UCVictoryCondition::Initialize(ACGameMode* inGameMode, ACGameState* inGameState)
{
	GameModeRef = inGameMode;
	GameStateRef = inGameState;
}

bool UCVictoryCondition::CheckVictoryCondition()
{
	if (!GameStateRef)
		return false;

	for (auto* Unit : GameModeRef->GetEnemyUnits())
	{
		UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(Unit);
		if (Attributes->GetHealth() > 0)
			return false;
	}
	return true;
}

bool UCVictoryCondition::CheckLossCondition()
{
	if (!GameStateRef)
		return false;

	for (auto* Unit : GameModeRef->GetHeroUnits())
	{
		UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(Unit);
		if (Attributes->GetHealth() > 0)
			return false;
	}
	return true;
}
