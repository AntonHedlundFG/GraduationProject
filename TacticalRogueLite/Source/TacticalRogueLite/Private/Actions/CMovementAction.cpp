// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CMovementAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utility/Logging/CLogManager.h"

void UCMovementAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	MovedUnit = Cast<ACUnit>(Instigator);
	if (!MovedUnit) return;

	FromTile = MovedUnit->GetTile();
	MovedUnit->SetTile(TargetTile);

	LOG_GAMEPLAY("%s moved.", *MovedUnit->GetUnitName());
}
void UCMovementAction::UndoAction(AActor* Instigator)
{
	if (!MovedUnit) return;

	MovedUnit->SetTile(FromTile);

	LOG_GAMEPLAY("%s returned.", *MovedUnit->GetUnitName());

	Super::UndoAction(Instigator);
}

TArray<ACGridTile*> UCMovementAction::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
	if (!GetOwningComponent())
	{
		LOG_WARNING("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
		return TArray<ACGridTile*>();	
	}

	//FGameplayTagContainer& Tags = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner())->ActiveGameplayTags;
	FGameplayTagContainer& MovementTags = GetOwningComponent()->ActiveGameplayTags;
	
	TArray<ACGridTile*> ReturnTiles;

	TSet<ACGridTile*> Tiles = UCGridUtilsLibrary::FloodFill(inTile, 2, MovementTags, ActionBlockingTags);
	
	Tiles.Remove(inTile);
	
	for (ACGridTile* Tile : Tiles)
	{
		ReturnTiles.Add(Tile);
	}

	return ReturnTiles;
}

void UCMovementAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCMovementAction, MovedUnit);
	DOREPLIFETIME(UCMovementAction, FromTile);
}