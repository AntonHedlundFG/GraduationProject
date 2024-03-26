// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CItem.h"
#include "Attributes/CAttributeComponent.h"
#include "GridContent/CUnit.h"

bool UCItem::IsValidTargetTile(ACUnit* inUnit, ACGridTile* inTargetTile)
{
	
	TArray<ACGridTile*> ValidTiles = GetValidTargetTiles(inUnit);
	return ValidTiles.Contains(inTargetTile);
}

TArray<ACGridTile*> UCItem::GetValidTargetTiles(ACUnit* inUnit)
{
	if (UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return TArray<ACGridTile*>();
	}

	return GetValidTargetTilesInternal(inUnit);
}

void UCItem::EquipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.AppendTags(GrantsTags);
	EquipOnUnitInternal(inUnit);
}

void UCItem::UnequipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.RemoveTags(GrantsTags);
	UnequipOnUnitInternal(inUnit);
}
