

#include "Items/CInventoryComponent.h"

#include "GridContent/CUnit.h"
#include "Items/ItemSlots.h"
#include "Net/UnrealNetwork.h"
#include "Actions/CActionComponent.h"
#include "GamePlayTags/SharedGameplayTags.h"
#include "ItemData/CItemData.h"

UCInventoryComponent::UCInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCInventoryComponent, Boots);
	DOREPLIFETIME(UCInventoryComponent, Weapon);
	DOREPLIFETIME(UCInventoryComponent, Armor);
	DOREPLIFETIME(UCInventoryComponent, Trinket);
	DOREPLIFETIME(UCInventoryComponent, AllItems);
}

TArray<UCItemData*> UCInventoryComponent::GetEquippedItems() const
{
	TArray<UCItemData*> AllEquipped;
	
	if (Boots != nullptr)
		AllEquipped.Add(Boots);
	if (Weapon != nullptr)
		AllEquipped.Add(Weapon);
	if (Armor != nullptr)
		AllEquipped.Add(Armor);
	if (Trinket != nullptr)
		AllEquipped.Add(Trinket);
	
	return AllEquipped;
}

UCItemData* UCInventoryComponent::GetItemInSlot(FGameplayTag inSlot)
{
	
	if (inSlot == SharedGameplayTags::ItemSlot_Boots)
		return Boots;
	if (inSlot == SharedGameplayTags::ItemSlot_Weapon)
		return Weapon;
	if (inSlot == SharedGameplayTags::ItemSlot_Armor)
		return Armor;
	if (inSlot == SharedGameplayTags::ItemSlot_Trinket)
		return Trinket;

	return nullptr;
}

bool UCInventoryComponent::TryEquipItem(UCItemData* inItem)
{
	if(inItem == nullptr)
		return false;
	const FGameplayTag SlotTag = inItem->ItemSlot;
	if (!CheckValidEquipmentTag(SlotTag))
		return false;

	UnEquipItem(SlotTag);
	
	if (SlotTag == SharedGameplayTags::ItemSlot_Boots)
		Boots = inItem;
	if (SlotTag == SharedGameplayTags::ItemSlot_Weapon)
		Weapon = inItem;
	if (SlotTag == SharedGameplayTags::ItemSlot_Armor)
		Armor = inItem;
	if (SlotTag == SharedGameplayTags::ItemSlot_Trinket)
		Trinket = inItem;

	AddItem(inItem);
	
	//OnItemChanged.Broadcast(this, inItem);
	
	return true;
}

void UCInventoryComponent::UnEquipItem(FGameplayTag inSlot)
{
	if (inSlot == SharedGameplayTags::ItemSlot_Boots)
	{
		RemoveItem(Boots);
		Boots = nullptr;
		return;
	}
	if (inSlot == SharedGameplayTags::ItemSlot_Weapon)
	{
		RemoveItem(Weapon);
		Weapon = nullptr;
		return;
	}
	if (inSlot == SharedGameplayTags::ItemSlot_Armor)
	{
		RemoveItem(Armor);
		Armor = nullptr;
		return;
	}
	if (inSlot == SharedGameplayTags::ItemSlot_Trinket)
	{
		RemoveItem(Trinket);
		Trinket = nullptr;
		return;
	}
}

void UCInventoryComponent::AddItem(UCItemData* inItem)
{
	if(ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		inItem->EquipOnUnit(Unit);
		AllItems.Add(inItem);
	}
}

void UCInventoryComponent::RemoveItem(UCItemData* inItem)
{
	if (!AllItems.Contains(inItem)) return;
	
	if(ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		inItem->UnequipOnUnit(Unit);
		AllItems.Remove(inItem);
	}
}

bool UCInventoryComponent::CheckValidEquipmentTag(FGameplayTag inTag)
{
	if (
		inTag == SharedGameplayTags::ItemSlot_Boots ||
		inTag == SharedGameplayTags::ItemSlot_Weapon ||
		inTag == SharedGameplayTags::ItemSlot_Armor ||
		inTag == SharedGameplayTags::ItemSlot_Trinket
		)
	{
		return true;
	}

	return false;
}


