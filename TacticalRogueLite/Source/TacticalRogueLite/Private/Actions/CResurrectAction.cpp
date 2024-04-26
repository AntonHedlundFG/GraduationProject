// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CResurrectAction.h"
#include "Net/UnrealNetwork.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "Attributes/CAttributeComponent.h"

void UCResurrectAction::StartAction(AActor* Instigator)
{
	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!GameState || !AffectedUnit || !AffectedUnit->GetAttributeComp() || !ResurrectOnTile) return;
	Super::StartAction(Instigator);

	PreviousTile = AffectedUnit->GetTile();
	AffectedUnit->SetTile(ResurrectOnTile);

	PreviousHealth = AffectedUnit->GetAttributeComp()->GetHealth();
	if (PreviousHealth < 1)
	{
		GameState->TurnOrder.Add(AffectedUnit);
		AffectedUnit->GetAttributeComp()->SetHealth(1);
		LOG_GAMEPLAY("%s returns from the dead.", *AffectedUnit->GetUnitName());
	}
	else
	{
		LOG_GAMEPLAY("%s moved on to the next room.", *AffectedUnit->GetUnitName());
	}


}

void UCResurrectAction::UndoAction(AActor* Instigator)
{
	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!GameState || !AffectedUnit || AffectedUnit->GetAttributeComp() || !ResurrectOnTile) return;

	if (PreviousHealth < 1)
	{
		AffectedUnit->GetAttributeComp()->SetHealth(PreviousHealth);
		GameState->TurnOrder.RemoveSingle(AffectedUnit);
	}

	AffectedUnit->SetTile(PreviousTile);

	LOG_GAMEPLAY("%s moved back.", *AffectedUnit->GetUnitName());

	Super::UndoAction(Instigator);
}

void UCResurrectAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCResurrectAction, AffectedUnit);
	DOREPLIFETIME(UCResurrectAction, ResurrectOnTile);
	DOREPLIFETIME(UCResurrectAction, PreviousTile);
	DOREPLIFETIME(UCResurrectAction, PreviousHealth);
}
