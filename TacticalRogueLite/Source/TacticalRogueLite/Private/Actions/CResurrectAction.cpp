// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CResurrectAction.h"
#include "Net/UnrealNetwork.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "Attributes/CAttributeComponent.h"
#include "Utility/Logging/CLogManager.h"
#include "Actions/CActionComponent.h"

void UCResurrectAction::StartAction(AActor* Instigator)
{
	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
	if (!GameState || !AffectedUnit || !AffectedUnit->GetAttributeComp() || !ResurrectOnTile) return;
	Super::StartAction(Instigator);

	PreviousTile = AffectedUnit->GetTile();
	AffectedUnit->SetTile(ResurrectOnTile);


	FAttribute HealthAttr, HealthMaxAttr;
	AffectedUnit->GetActionComp()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.Health"), HealthAttr);
	AffectedUnit->GetActionComp()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.HealthMax"), HealthMaxAttr);
	int CurrentHealth = HealthAttr.BaseValue;
	int MaxHealth = HealthMaxAttr.BaseValue;
	int MinResurrectHealth;

	if (bUsePercentage)
		MinResurrectHealth = FMath::Max(1, MaxHealth * ResurrectPercentageHealth);
	else
		MinResurrectHealth = ResurrectFlatHealth;

	if (CurrentHealth < MinResurrectHealth)
	{
		FAttributeModification HealthMod;
		HealthMod.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
		HealthMod.ModifierOperation = EAttributeModifierOperation::OverrideBase;
		HealthMod.Magnitude = MinResurrectHealth;
		AffectedUnit->GetActionComp()->ApplyAttributeChange(HealthMod, 0);
	}

	if (!GameState->TurnOrder.Contains(AffectedUnit))
	{
		const int RandomTurnOrderIndex = FMath::RandRange(1, GameState->TurnOrder.Num());
		GameState->TurnOrder.Insert(AffectedUnit, RandomTurnOrderIndex);
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
