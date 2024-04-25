// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CMovementAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"

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

	FGameplayTagContainer& Tags = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner())->ActiveGameplayTags;
	
	TArray<ACGridTile*> ReturnTiles;
	ReturnTiles.Add(inTile);

	FGameplayTag UnitParentTag = FGameplayTag::RequestGameplayTag("Unit");
	FGameplayTagContainer UnitBlock = UGameplayTagsManager::Get().RequestGameplayTagChildren(UnitParentTag);
	for (ACGridTile* Tile : UCGridUtilsLibrary::FloodFill(inTile, 2, Tags, UnitBlock))
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