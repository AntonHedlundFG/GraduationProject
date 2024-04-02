

#include "Items/CInventoryComponent.h"

#include "GridContent/CUnit.h"
#include "Items/CItem.h"
#include "Items/ItemSlots.h"
#include "Net/UnrealNetwork.h"
#include "Actions/CActionComponent.h"
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
	DOREPLIFETIME(UCInventoryComponent, Helmet);
	DOREPLIFETIME(UCInventoryComponent, Ring);
	DOREPLIFETIME(UCInventoryComponent, AllItems);
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

bool UCInventoryComponent::TryEquipItem(UCItem* inItem, EItemSlots inSlot)
{
	const EItemSlots slot = inSlot == EItemSlots::EIS_None? inItem->ItemSlot : inSlot;
	bool bSuccess = false;

	UnEquipItem(slot);
	
	switch (slot)
	{
	case EItemSlots::EIS_Boots:
		Boots = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Weapon:
		Weapon = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Body:
		Armor = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Helmet:
		Helmet = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Ring:
		Ring = inItem;
		bSuccess = true;
		break;
	default:
		break;
	}

	if (bSuccess)
	{
		AddItem(inItem);
	}
	return bSuccess;
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

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() >= ENetMode::NM_Client) return;

	if (ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		if (Unit->GetActionComp() && TESTITEM)
			Unit->GetActionComp()->AddAbility(TESTITEM->Ability);
	}

}

