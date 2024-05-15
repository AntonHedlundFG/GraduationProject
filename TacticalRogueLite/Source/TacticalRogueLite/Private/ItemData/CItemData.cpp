// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemData/CItemData.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Attributes/CAttributeComponent.h"


void UCItemData::EquipOnUnit(ACUnit* inUnit) //TODO: Handle stacking for actioneffects?
{
	
	UCActionComponent* ActionComp = inUnit->GetActionComp();

	if (ActionComp)
	{
		ActionComp->ActiveGameplayTags.AppendTags(OwnedTags);
		
		for (FAttributeModification Mod : AttributeModifications)
		{
			Mod.InstigatorComp = ActionComp;
			ActionComp->ApplyAttributeChange(Mod, 0);
		}
		//If Charm item.. Temporary solution.. Maybe normal items should fire these actions too, if they have any. 
		if (ItemSlot == FGameplayTag::RequestGameplayTag(FName("ItemSlot.Charm")))
		{
			if (InstantActions.IsEmpty())
			{
				return;
			}
			//TODO: Change to softrefs and loadsynchronous?
			//Add and start(if thats the case)all "lose" actions this charm is holding. 
			for (TSubclassOf<UCAction> Action : InstantActions) //all the actions in this list should be true.
			{
				ActionComp->AddAction(inUnit, Action);
			}
		}
		else //If this is an item that only carries ability items.  
		{
			Ability.InventorySlotTag = ItemSlot;
			ActionComp->AddAbility(Ability); //all the actions in the ability struct is false.
		}
	}

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(inUnit);
	if (Attributes)
	{
		Attributes->AddMaxCharges(ItemSlot, 1);
	}
}

void UCItemData::UnequipOnUnit(ACUnit* inUnit) //TODO: Remove instant actions/stop.
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

		for (FAttributeModification Mod : AttributeModifications)
		{
			Mod.Magnitude = -Mod.Magnitude;
			ActionComp->ApplyAttributeChange(Mod, 0);
		}
	}
}

