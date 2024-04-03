// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CMovementAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"

void UCMovementAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	MovedUnit = Cast<ACUnit>(Instigator);
	if (!MovedUnit) return;

	FromTile = MovedUnit->GetTile();
	MovedUnit->SetTile(TargetTile);

	UCLogManager::Log(
		ELogCategory::LC_Gameplay, 
		MovedUnit->GetUnitName().Append(" moved.")
	);

}
void UCMovementAction::UndoAction_Implementation(AActor* Instigator)
{
	Super::UndoAction_Implementation(Instigator);

	if (!MovedUnit) return;

	MovedUnit->SetTile(FromTile);

	UCLogManager::Log(
		ELogCategory::LC_Gameplay,
		MovedUnit->GetUnitName().Append(" returned.")
	);
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
	for (ACGridTile* Tile : UCGridUtilsLibrary::FloodFill(inTile, 2, Tags))
	{
		ReturnTiles.Add(Tile);
	}

	return ReturnTiles;
}
