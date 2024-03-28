// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/CItemData.h"

#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Attributes/CAttributeComponent.h"


void UCItemData::EquipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.AppendTags(GrantsTags); //TODO: Change to gameplaytagcountcontainer

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		ActionComp->AddAbility(ActionClasses, ItemSlot);
	}
}

void UCItemData::UnequipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent::GetAttributes(inUnit)->ActiveGameplayTags.RemoveTags(GrantsTags);

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		ActionComp->RemoveAbility(ItemSlot);
	}
}

FPrimaryAssetId UCItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
