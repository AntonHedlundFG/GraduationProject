

#include "Items/CInventoryComponent.h"

#include "GridContent/CUnit.h"
#include "Items/CItem.h"
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
	DOREPLIFETIME(UCInventoryComponent, Helmet);
	DOREPLIFETIME(UCInventoryComponent, Ring);
	DOREPLIFETIME(UCInventoryComponent, AllItems);
}

TArray<UCItemData*> UCInventoryComponent::GetEquippedItems() const
{
	TArray<UCItemData*> AllEquipped;
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

UCItemData* UCInventoryComponent::GetItemInSlot(FGameplayTag inSlot)
{
	EItemSlots slot = ConvertTagToSlot(inSlot);
	
	switch (slot)
	{
	case EItemSlots::EIS_Boots:
		return Boots;
	case EItemSlots::EIS_Weapon:
		return Weapon;
	case EItemSlots::EIS_Armor:
		return Armor;
	case EItemSlots::EIS_Helmet:
		return Helmet;
	case EItemSlots::EIS_Ring:
		return Ring;
	default:
		return nullptr;
	}
}

bool UCInventoryComponent::TryEquipItem(UCItemData* inItem)
{
	const FGameplayTag SlotTag = inItem->ItemSlot;
	UnEquipItem(SlotTag);
	EItemSlots Slot = ConvertTagToSlot(SlotTag);
	bool bSuccess = false;


	switch (Slot)
	{
	case EItemSlots::EIS_Boots:
		Boots = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Weapon:
		Weapon = inItem;
		bSuccess = true;
		break;
	case EItemSlots::EIS_Armor:
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

void UCInventoryComponent::UnEquipItem(FGameplayTag inSlot)
{
	EItemSlots Slot = ConvertTagToSlot(inSlot);
	
	switch (Slot)
	{
	case EItemSlots::EIS_Boots:
		RemoveItem(Boots);
		Boots = nullptr;
		break;
	case EItemSlots::EIS_Weapon:
		RemoveItem(Weapon);
		Weapon = nullptr;
		break;
	case EItemSlots::EIS_Armor:
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
		break;
	default:
		break;
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

EItemSlots UCInventoryComponent::ConvertTagToSlot(FGameplayTag tag)
{
	if (tag == SharedGameplayTags::ItemSlot_Boots)
	{
		return EItemSlots::EIS_Boots;
	}
	if (tag == SharedGameplayTags::ItemSlot_Weapon)
	{
		return EItemSlots::EIS_Weapon;
	}
	if (tag == SharedGameplayTags::ItemSlot_Armor)
	{
		return EItemSlots::EIS_Armor;
	}
	if (tag == SharedGameplayTags::ItemSlot_Helmet)
	{
		return EItemSlots::EIS_Helmet;
	}
	if (tag == SharedGameplayTags::ItemSlot_Ring)
	{
		return EItemSlots::EIS_Ring;
	}
	if (tag == SharedGameplayTags::ItemSlot_Charm)
	{
		return EItemSlots::EIS_None;
	}
	return EItemSlots::EIS_MAX;
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() >= ENetMode::NM_Client) return;

	if (ACUnit* Unit = Cast<ACUnit>(GetOwner()))
	{
		if (Unit->GetActionComp() && TESTITEM)
			Unit->GetActionComp()->AddAbility(TESTITEM->Ability);
		if (Unit->GetActionComp() && TESTATTACKITEM)
			Unit->GetActionComp()->AddAbility(TESTATTACKITEM->Ability);
	}

}

