// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CDeathAction.h"
#include "CGameState.h"
#include "GridContent/CUnit.h"

void UCDeathAction::StartAction_Implementation(AActor* Instigator)
{
	if (!AffectedUnit)
	{
		LOG_WARNING("AffectedUnit has not been set, cannot kill nullptrs");
		return;
	}

	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!GameState)
	{
		LOG_WARNING("No GameState reference available");
		return;
	}

	DeathTurnOrderIndex = GameState->TurnOrder.Find(AffectedUnit);
	if (DeathTurnOrderIndex == INDEX_NONE)
	{
		LOG_WARNING("Unit was not in turn order");
	}
	else
	{
		GameState->TurnOrder.RemoveAt(DeathTurnOrderIndex);
	}

	DeathTile = AffectedUnit->GetTile();

	AffectedUnit->SetTile(nullptr);
	AffectedUnit->SetActorLocation(FVector::OneVector * 10000);

	UCLogManager::Log(
		ELogCategory::LC_Gameplay,
		AffectedUnit->GetUnitName().Append(" died.")
	);

}

void UCDeathAction::UndoAction_Implementation(AActor* Instigator)
{
	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!AffectedUnit || !GameState)
	{
		LOG_WARNING("Lacking unit or state reference for undoing death");
		return;
	}

	AffectedUnit->SetTile(DeathTile);
	if (DeathTurnOrderIndex != INDEX_NONE)
		GameState->TurnOrder.Insert(AffectedUnit, DeathTurnOrderIndex);

	UCLogManager::Log(
		ELogCategory::LC_Gameplay,
		AffectedUnit->GetUnitName().Append(" resurrected.")
	);
}
