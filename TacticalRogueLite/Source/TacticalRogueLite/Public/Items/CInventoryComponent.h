
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/CItemData.h"
#include "ItemData/CItemData_Charm.h"
#include "CInventoryComponent.generated.h"

class UCItem;
class UCItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, UCInventoryComponent*, OwningComp, UCItemData*, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryCharmEvent, UCItemData*, Charm, int32, Quantity);

//Represents an item slot that can be stackable.
USTRUCT(BlueprintType)
struct TACTICALROGUELITE_API FSlot
{
	GENERATED_BODY()

	FSlot()
	{
		OwnerInventory = nullptr;
		ItemInstance = nullptr;
		Quantity = 0;
	}

	FSlot(UCItemData* InItemInstance, const int32 InQuantity, UCInventoryComponent* InOwnerInventory)
	{
		ItemInstance = InItemInstance;
		Quantity = InQuantity;
		OwnerInventory = InOwnerInventory;
		
	}

	UPROPERTY(BlueprintReadOnly)
	UCItemData* ItemInstance;

	UPROPERTY(BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(BlueprintReadOnly)
	UCInventoryComponent* OwnerInventory;
	

	bool operator == (const FSlot& Other) const
	{
		return Other.ItemInstance == ItemInstance && Other.Quantity == Quantity;
	}

	bool operator != (const FSlot& Other) const
	{
		return Other.ItemInstance != ItemInstance && Other.Quantity != Quantity;
	}

	bool IsOnMaxStackSize() const;
	int32 GetMissingStackQuantity() const;

	void SetQuantity(int32 InQuantity);
	void UpdateQuantity(int32 InQuantity);

	bool IsEmpty() const;
	bool IsOccupied() const;
	bool IsValid() const;

	bool IsValidSlot(const FSlot& Slot);
	bool IsEmptySlot(const FSlot& Slot);
	bool IsOccupiedSlot(const FSlot& Slot);
	bool IsSlotOnMaxStackSize(const FSlot& Slot);
	int32 GetSlotMissingStackQuantity(const FSlot& Slot);

	UPROPERTY(BlueprintAssignable)	
	FInventoryCharmEvent OnCharmAdded;

	UPROPERTY(BlueprintAssignable)	
	FInventoryCharmEvent OnCharmRemoved;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALROGUELITE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Items")
	TArray<UCItemData*>GetAllItems() const { return AllItems; }
	UFUNCTION(BlueprintCallable, Category = "Items|Equipment")
	TArray<UCItemData*> GetEquippedItems() const;
	UFUNCTION(BlueprintCallable, Category = "Items|Equipment")
	UCItemData* GetItemInSlot(FGameplayTag inSlot);

	//No need to call Add/Remove Item function when using one of these
	UFUNCTION(Category = "Items|Equipment")
	bool TryEquipItem(UCItemData* inItem);
    UFUNCTION(Category = "Items|Equipment")
    void UnEquipItem(FGameplayTag inSlot);

	//Use for adding "charms", these will be automatically called in Equip/UnEquip functions respectively
	UFUNCTION(Category = "Items")
	void AddItem(UCItemData* inItem);
	UFUNCTION(Category = "Items")
	void RemoveItem(UCItemData* inItem);
	UFUNCTION(Category = "Items|Equipment")
	bool CheckValidEquipmentTag(FGameplayTag inTag);

	UPROPERTY(BlueprintAssignable)
	FOnItemChanged OnItemChanged;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items")
	TArray<UCItemData*> AllItems;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItemData> Boots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItemData> Weapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItemData> Armor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItemData> Trinket;
	
	
	// ---- CHARMS ---- //

#pragma region CharmItem Charges

	//Check if a charm is already equipped. 
	bool DoesItemExist(const UCItemData* Item, FSlot& OutSlot);

	//Check if we have any blocking tags that makes us unable to carry the charm. 
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool CanCarryCharm(const UCItemData_Charm* Charm, const int32 Quantity) const;

	//Probably not neccessary since we equip/unequip the same way as the other items?
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveCharm(UCItemData_Charm* Charm, int32 Quantity, int32& RemovedQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveCharmOnSlot(const FSlot& Slot, int32 Quantity, int32& RemovedQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void StackCharmStackOnSlot(const FSlot& Slot, int32 Quantity);

	// UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Inventory Item Used"), Category = "Inventory")
	// void K2_OnInventoryItemUsed(UItem* Item, int32 Quantity);

	//List of all slots. 
	UPROPERTY(BlueprintReadWrite, Category = "Inventory", Replicated)
	TArray<FSlot> Slots;

#pragma endregion CharmItem Charges
	
};



