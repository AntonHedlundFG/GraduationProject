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
		Attributes->ActiveGameplayTags.AppendTags(OwnedTags);
		Attributes->AddMaxCharges(ItemSlot, 1);
	}

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		Ability.InventorySlotTag = ItemSlot;
		ActionComp->AddAbility(Ability);
	}
}

void UCItemData::UnequipOnUnit(ACUnit* inUnit)
{
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(inUnit);
	if (Attributes)
	{
		//Append the items ownedtags to the attributecomp.
		Attributes->ActiveGameplayTags.RemoveTags(OwnedTags);
		Attributes->RemoveMaxCharges(ItemSlot, 1);
	}

	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		ActionComp->RemoveAbility(ItemSlot);
	}
}

