

#include "Items/CInventoryComponent.h"

#include "Items/CItem.h"
#include "Items/ItemSlots.h"

UCInventoryComponent::UCInventoryComponent()
{
	
}

TArray<UCItem*> UCInventoryComponent::GetEquippedItems() const
{
	TArray<UCItem*> AllEquipped;
	if (Boots != nullptr)
	{
		AllEquipped.Add(Boots);
	}
	if (Weapon != nullptr)
	{
		AllEquipped.Add(Weapon);
	}
	if (Armor != nullptr)
	{
		AllEquipped.Add(Armor);
	}
	if (Helmet != nullptr)
	{
		AllEquipped.Add(Helmet);
	}
	if (Ring != nullptr)
	{
		AllEquipped.Add(Ring);
	}
	
	return AllEquipped;
}

UCItem* UCInventoryComponent::GetItemInSlot(EItemSlots inSlot) const
{
	switch (inSlot)
	{
	case EItemSlots::EIS_Boots:
		return Boots;
	case EItemSlots::EIS_Weapon:
		return Weapon;
	case EItemSlots::EIS_Body:
		return Armor;
	case EItemSlots::EIS_Helmet:
		return Helmet;
	case EItemSlots::EIS_Ring:
		return Ring;
	default:
		return nullptr;
	}
}

TArray<UCItem*> UCInventoryComponent::GetAllItems() const
{
	TArray<UCItem*> Items = Charms;
	TArray<UCItem*> Equipped = GetEquippedItems();
	for (auto item : Equipped)
	{
		if (!Items.Contains(item))
		{
			Items.Add(item);
		}
	}
	return Items;
}

void UCInventoryComponent::EquipItem(UCItem* inItem, EItemSlots inSlot)
{
	EItemSlots slot = inSlot == EItemSlots::EIS_None? inItem->ItemSlot : inSlot;
	switch (slot)
	{
	case EItemSlots::EIS_Boots:
		Boots = inItem;
		break;
	case EItemSlots::EIS_Weapon:
		Weapon = inItem;
		break;
	case EItemSlots::EIS_Body:
		Armor = inItem;
		break;
	case EItemSlots::EIS_Helmet:
		Helmet = inItem;
		break;
	case EItemSlots::EIS_Ring:
		Ring = inItem;
		break;
	default:
		break;
	}
	
	AddCharm(inItem);
}

void UCInventoryComponent::AddCharm(UCItem* inItem)
{
	//CHECK HERE IF ITEM IS CHARM / HAS PASSIVE EFFECTS
	bool isCharm = false;
	
	if (isCharm)
	{
		Charms.Add(inItem);
	}
}

