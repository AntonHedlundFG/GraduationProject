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

}
void UCMovementAction::UndoAction_Implementation(AActor* Instigator)
{
	Super::UndoAction_Implementation(Instigator);

	if (!MovedUnit) return;

	MovedUnit->SetTile(FromTile);

}

TArray<ACGridTile*> UCMovementAction::GetValidTargetTiles(ACGridTile* inTile)
{
	//auto& Tags = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner())->ActiveGameplayTags;
	FGameplayTagContainer Tags;
	
	UE_LOG(LogTemp, Warning, TEXT("HERETWO"));

	TArray<ACGridTile*> ReturnTiles;
	for (ACGridTile* Tile : UCGridUtilsLibrary::FloodFillFromTag(Tags, inTile, 2))
	{
		ReturnTiles.Add(Tile);
		UE_LOG(LogTemp, Warning, TEXT("HERE"));
	}

	return ReturnTiles;
}
