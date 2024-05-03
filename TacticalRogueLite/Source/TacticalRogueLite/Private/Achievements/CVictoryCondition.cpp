// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievements/CVictoryCondition.h"
#include "CGameState.h"
#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Attributes/Utilities/CAttribute.h"

void UCVictoryCondition::Initialize(ACGameMode* inGameMode, ACGameState* inGameState)
{
	GameModeRef = inGameMode;
	GameStateRef = inGameState;
}

bool UCVictoryCondition::CheckVictoryCondition()
{
	if (!GameModeRef)
		return false;

	if (bWonByCheating) return true;

	for (auto* Unit : GameModeRef->GetEnemyUnits())
	{
		FAttribute Health;
		if (Unit->GetActionComp()->GetAttribute(FGameplayTag::RequestGameplayTag(FName("Attribute.Health")), Health))
		{
			if (Health.BaseValue > 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool UCVictoryCondition::CheckLossCondition()
{
	if (!GameModeRef)
		return false;

	if (bLoseByCheating) return true;

	for (auto* Unit : GameModeRef->GetHeroUnits())
	{
		FAttribute Health;
		if (Unit->GetActionComp()->GetAttribute(FGameplayTag::RequestGameplayTag(FName("Attribute.Health")), Health))
		{
			if (Health.BaseValue > 0)
			{
				return false;
			}
		}
	}
	return true;
}
