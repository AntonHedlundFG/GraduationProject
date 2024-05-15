

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
	DOREPLIFETIME(UCInventoryComponent, Slots);
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
	
	if (inSlot == TAG_ItemSlot_Boots)
		return Boots;
	if (inSlot == TAG_ItemSlot_Weapon)
		return Weapon;
	if (inSlot == TAG_ItemSlot_Armor)
		return Armor;
	if (inSlot == TAG_ItemSlot_Trinket)
		return Trinket;

	return nullptr;
}

bool UCInventoryComponent::TryEquipItem(UCItemData* inItem) //Charm is not added to "all items"- list atm, also needs: Events to update UI etc
{
	int AddedQuantity = 0;
	
	if(inItem == nullptr)
	{
		return false;
	}
	
	const FGameplayTag SlotTag = inItem->ItemSlot;

	//If the added item is a charm.
	if(inItem->ItemSlot == FGameplayTag::RequestGameplayTag("ItemSlot.Charm")) //Temp solution..
	{
		int32 RemainingQuantity = 2; //Change.
		if (inItem->bCanBeStacked)
		{
			//Do we already own the item?
			FSlot FoundSlot;
			if (DoesItemExist(inItem, FoundSlot))
			{
				//Can it be stacked?
				const bool bCanStack = !FoundSlot.IsOnMaxStackSize() && FoundSlot.ItemInstance == inItem; //Second redudant?
				if (bCanStack)
				{
					const int32 MissingStackQuantity = FoundSlot.GetMissingStackQuantity();

					if (RemainingQuantity <= MissingStackQuantity)
					{
						//Rules to not be able to equip items? Blocked tags..?
							
						FoundSlot.UpdateQuantity(MissingStackQuantity);
						AddedQuantity += MissingStackQuantity;
						RemainingQuantity -= MissingStackQuantity;

						return true; //true..
					}
				}
			}
			//If new item.
			if (RemainingQuantity > inItem->MaxStackSize)
			{
				//Create new slot for item.
				FSlot NewSlot = FSlot(inItem, inItem->MaxStackSize, this);
				Slots.Add(NewSlot);

				AddedQuantity += inItem->MaxStackSize;
				RemainingQuantity -= inItem->MaxStackSize;

				return true;
			}
		}
		//If cannot be stacked..
		if (RemainingQuantity > 0)
		{
			//Create instance..?

			const FSlot NewSlot = FSlot(inItem, 1, this);
			Slots.Add(NewSlot);

			AddedQuantity += 1;
			RemainingQuantity -= 1;

			//Update events..

			return true;
		}
	}
		
		if (!CheckValidEquipmentTag(SlotTag))
		{
			return false;
		}
		
		UnEquipItem(SlotTag);
	
		if (SlotTag == TAG_ItemSlot_Boots)
			Boots = inItem;
		if (SlotTag == TAG_ItemSlot_Weapon)
			Weapon = inItem;
		if (SlotTag == TAG_ItemSlot_Armor)
			Armor = inItem;
		if (SlotTag == TAG_ItemSlot_Trinket)
			Trinket = inItem;

		AddItem(inItem);
	
		OnItemChanged.Broadcast(this, inItem);
	
		return true;
	
}

void UCInventoryComponent::UnEquipItem(FGameplayTag inSlot) //Charms need to be able to be unequipped.
{
	if (inSlot == TAG_ItemSlot_Boots)
	{
		RemoveItem(Boots);
		Boots = nullptr;
		return;
	}
	if (inSlot == TAG_ItemSlot_Weapon)
	{
		RemoveItem(Weapon);
		Weapon = nullptr;
		return;
	}
	if (inSlot == TAG_ItemSlot_Armor)
	{
		RemoveItem(Armor);
		Armor = nullptr;
		return;
	}
	if (inSlot == TAG_ItemSlot_Trinket)
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
		inTag == TAG_ItemSlot_Boots ||
		inTag == TAG_ItemSlot_Weapon ||
		inTag == TAG_ItemSlot_Armor ||
		inTag == TAG_ItemSlot_Trinket 
		)
	{
		return true;
	}

	return false;
}


#pragma region CharmItem Charges

bool UCInventoryComponent::DoesItemExist(const UCItemData* Item, FSlot& OutSlot)
{
	for (FSlot& Slot : Slots)
	{
		if (Slot.ItemInstance == Item)
		{
			OutSlot = Slot;
			return true;
		}
	}

	return false;
}

bool UCInventoryComponent::CanCarryCharm(const UCItemData_Charm* Item, const int32 Quantity) const
{
	//if blocked tags?
	return false;
}


bool UCInventoryComponent::RemoveCharm(UCItemData_Charm* Charm, int32 Quantity, int32& RemovedQuantity)
{
	return true;

}

bool UCInventoryComponent::RemoveCharmOnSlot(const FSlot& Slot, int32 Quantity, int32& RemovedQuantity)
{
	return true;
}

void UCInventoryComponent::StackCharmStackOnSlot(const FSlot& Slot, int32 Quantity)
{
	
}

#pragma endregion

bool FSlot::IsOnMaxStackSize() const
{
	if (ItemInstance == nullptr)
	{
		return false;
	}

	if (ItemInstance->bCanBeStacked)
	{
		if (Quantity >= ItemInstance->MaxStackSize)
		{
			return true;
		}

		return false;
	}

	return false;
}

int32 FSlot::GetMissingStackQuantity() const
{
	if (ItemInstance == nullptr)
	{
		return 0;
	}

	if (ItemInstance->bCanBeStacked)
		
	{
		return ItemInstance->MaxStackSize - Quantity;
	}

	return 0;
}

void FSlot::SetQuantity(const int32 InQuantity)
{
	if (ItemInstance == nullptr)
	{
		return;
	}
	
	if (ItemInstance->bCanBeStacked)
	{
		Quantity = FMath::Clamp(InQuantity, 0, ItemInstance->MaxStackSize);
	}
	else
	{
		Quantity = FMath::Clamp(InQuantity, 0, 1);
	}
}

void FSlot::UpdateQuantity(const int32 InQuantity)
{
	if (ItemInstance == nullptr)
	{
		return;
	}
	
	if (ItemInstance->bCanBeStacked)
	{
		Quantity = FMath::Clamp(Quantity + InQuantity, 0, ItemInstance->MaxStackSize);
	}
	else
	{
		Quantity = FMath::Clamp(Quantity + InQuantity, 0, 1);
	}
}

bool FSlot::IsEmpty() const
{
	return (ItemInstance == nullptr && Quantity == 0 && OwnerInventory != nullptr);
}

bool FSlot::IsOccupied() const
{
	return (ItemInstance != nullptr && Quantity > 0 && OwnerInventory != nullptr);
}

bool FSlot::IsValid() const
{
	return (OwnerInventory != nullptr && Quantity >= 0);
}

bool FSlot::IsValidSlot(const FSlot& Slot)
{
	return true;
}

bool FSlot::IsEmptySlot(const FSlot& Slot)
{
	return true;
}

bool FSlot::IsOccupiedSlot(const FSlot& Slot)
{
	return true;
}

bool FSlot::IsSlotOnMaxStackSize(const FSlot& Slot)
{
	return true;
}

int32 FSlot::GetSlotMissingStackQuantity(const FSlot& Slot)
{
	return 0;
}




