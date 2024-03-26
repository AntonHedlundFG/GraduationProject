

#include "Items/CInventoryComponent.h"

#include "GridContent/CUnit.h"
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
	
	AddItem(inItem);
}

void UCInventoryComponent::UnEquipItem(EItemSlots inSlot)
{
	
	switch (inSlot)
	{
	case EItemSlots::EIS_Boots:
		RemoveItem(Boots);
		Boots = nullptr;
		break;
	case EItemSlots::EIS_Weapon:
		RemoveItem(Weapon);
		Weapon = nullptr;
		break;
	case EItemSlots::EIS_Body:
		RemoveItem(Armor);
		Armor = nullptr;
		break;
	case EItemSlots::EIS_Helmet:
		RemoveItem(Helmet);
		Helmet = nullptr;
		break;
	case EItemSlots::EIS_Ring:
		RemoveItem(Ring);
		Ring = nullptr;
		break;
	default:
		break;
	}
}

void UCInventoryComponent::AddItem(UCItem* inItem)
{
	if(ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		inItem->EquipOnUnit(Unit);
		AllItems.Add(inItem);
	}
}

void UCInventoryComponent::RemoveItem(UCItem* inItem)
{
	if (!AllItems.Contains(inItem)) return;
	
	if(ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		inItem->UnequipOnUnit(Unit);
		AllItems.Remove(inItem);
	}
}

