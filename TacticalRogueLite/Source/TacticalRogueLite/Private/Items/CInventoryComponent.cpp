

#include "Items/CInventoryComponent.h"

#include "Items/ItemSlots.h"

UCInventoryComponent::UCInventoryComponent()
{
	
}

TArray<UCItem*> UCInventoryComponent::GetAllEquipment() const
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
	switch (inSlot)
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

