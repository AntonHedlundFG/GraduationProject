// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/CItemData.h"

#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Attributes/CAttributeComponent.h"


void UCItemData::EquipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(inUnit);
	if (Attributes)
	{
		Attributes->AddMaxCharges(ItemSlot, 1);
	}

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		ActionComp->ActiveGameplayTags.AppendTags(OwnedTags);
		Ability.InventorySlotTag = ItemSlot;
		ActionComp->AddAbility(Ability);
	}
}

void UCItemData::UnequipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(inUnit);
	if (Attributes)
	{
		Attributes->RemoveMaxCharges(ItemSlot, 1);
	}

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		//Append the items ownedtags to the ActionComp.
		ActionComp->ActiveGameplayTags.RemoveTags(OwnedTags);
		ActionComp->RemoveAbility(ItemSlot);
	}
}

