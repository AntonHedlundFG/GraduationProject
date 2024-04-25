// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CDeathAction.h"
#include "CGameState.h"
#include "Attributes/CAttributeComponent.h"
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"

void UCDeathAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

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
		GameState->OnRep_TurnOrder();
	}

	DeathTile = AffectedUnit->GetTile();

	AffectedUnit->SetTile(nullptr);
	AffectedUnit->GetAttributeComp()->SetIsPendingKill(false);

	LOG_GAMEPLAY("%s died.", *AffectedUnit->GetUnitName());

}

void UCDeathAction::UndoAction(AActor* Instigator)
{

	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!AffectedUnit || !GameState)
	{
		LOG_WARNING("Lacking unit or state reference for undoing death");
		return;
	}

	AffectedUnit->SetTile(DeathTile);
	if (DeathTurnOrderIndex != INDEX_NONE)
	{
		GameState->TurnOrder.Insert(AffectedUnit, DeathTurnOrderIndex);
		GameState->OnRep_TurnOrder();
	}

	LOG_GAMEPLAY("%s resurrected.", *AffectedUnit->GetUnitName());

	AffectedUnit->GetAttributeComp()->SetIsPendingKill(false);

	Super::UndoAction(Instigator);

}

void UCDeathAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCDeathAction, AffectedUnit);
}