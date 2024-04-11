// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CAttackAction.h"

#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"
#include "Attributes/CGameplayFunctionLibrary.h"
#include "Grid/CGridTile.h"
#include "Grid/CGridUtilsLibrary.h"
#include "GridContent/CUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void UCAttackAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AttackingUnit = Cast<ACUnit>(Instigator);
	TargetUnit = Cast<ACUnit>(TargetTile->GetContent());

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
	OldHealth = Attributes->GetHealth();

	UCGameplayFunctionLibrary::ApplyDamage(Instigator, TargetTile->GetContent(), DamageAmount);
}

void UCAttackAction::UndoAction_Implementation(AActor* Instigator)
{
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
	Attributes->SetHealth(OldHealth);
	
	Super::UndoAction_Implementation(Instigator);
}

TArray<ACGridTile*> UCAttackAction::GetValidTargetTiles(ACGridTile* inTile)
{
	
	if (!GetOwningComponent())
	{
		LOG_WARNING("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
		return TArray<ACGridTile*>();	
	}
	

	TArray<ACGridTile*> ReturnTiles;
	for (ACGridTile* Tile : inTile->GetNeighbours(false))
	{
		if (Tile->GetContent() != nullptr && Tile->GetContent()->IsA(ACUnit::StaticClass()))
		{
			ReturnTiles.Add(Tile);
		}
	}

	return ReturnTiles;
	
}
void UCAttackAction::PrintStartMessage()
{
	LOG_GAMEPLAY("%s attacked %s for %d damage.",
		AttackingUnit ? *AttackingUnit->GetUnitName() : *FString("Unknown Unit"),
		AttackingUnit ? *AttackingUnit->GetUnitName() : *FString("Unknown Unit"),
		DamageAmount
	);
}
void UCAttackAction::PrintUndoMessage()
{
	LOG_GAMEPLAY("%s undid their attack on %s for %d damage.",
		AttackingUnit ? *AttackingUnit->GetUnitName() : *FString("Unknown Unit"),
		AttackingUnit ? *AttackingUnit->GetUnitName() : *FString("Unknown Unit"),
		DamageAmount
	);
}

void UCAttackAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAttackAction, AttackingUnit);
	DOREPLIFETIME(UCAttackAction, TargetUnit);
	DOREPLIFETIME(UCAttackAction, DamageAmount);
}