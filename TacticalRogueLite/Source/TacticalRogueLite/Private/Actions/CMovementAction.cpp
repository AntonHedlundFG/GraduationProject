// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CMovementAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"

void UCMovementAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	MovedUnit = Cast<ACUnit>(Instigator);
	if (!MovedUnit) return;

	FromTile = MovedUnit->GetTile();
	MovedUnit->SetTile(TargetTile);
}
void UCMovementAction::UndoAction_Implementation(AActor* Instigator)
{
	Super::UndoAction_Implementation(Instigator);

	if (!MovedUnit) return;

	MovedUnit->SetTile(FromTile);
}

TArray<ACGridTile*> UCMovementAction::GetValidTargetTiles(ACGridTile* inTile)
{
	if (!GetOwningComponent())
	{
		LOG_WARNING("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
		return TArray<ACGridTile*>();	
	}

	FGameplayTagContainer& Tags = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner())->ActiveGameplayTags;

	TArray<ACGridTile*> ReturnTiles;
	ReturnTiles.Add(inTile);
	for (ACGridTile* Tile : UCGridUtilsLibrary::FloodFill(inTile, 2, Tags))
	{
		ReturnTiles.Add(Tile);
	}

	return ReturnTiles;
}

void UCMovementAction::PrintStartMessage()
{
	LOG_GAMEPLAY("%s moved.", MovedUnit ? *MovedUnit->GetUnitName() : *FString("Unknown Unit"));
}

void UCMovementAction::PrintUndoMessage()
{
	LOG_GAMEPLAY("%s returned.", MovedUnit ? *MovedUnit->GetUnitName() : *FString("Unknown Unit"));
}

void UCMovementAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCMovementAction, MovedUnit);
}
