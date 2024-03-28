
#pragma once

#include "CoreMinimal.h"
#include "ItemSlots.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"


enum class EItemSlots : uint8;
class UCItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();
	
	UFUNCTION(Category = "Items")
	TArray<UCItem*>GetAllItems() const { return AllItems; }
	UFUNCTION(Category = "Items|Equipment")
	TArray<UCItem*> GetEquippedItems() const;
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItemInSlot(EItemSlots inSlot) const;

	//No need to call Add/Remove Item function when using one of these
	UFUNCTION(Category = "Items|Equipment")
	bool TryEquipItem(UCItem* inItem, EItemSlots inSlot = EItemSlots::EIS_None);
    UFUNCTION(Category = "Items|Equipment")
    void UnEquipItem(EItemSlots inSlot);

	//Use for adding "charms", these will be automatically called in Equip/UnEquip functions respectively
	UFUNCTION(Category = "Items")
	void AddItem(UCItem* inItem);
	UFUNCTION(Category = "Items")
	void RemoveItem(UCItem* inItem);


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItem> Boots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItem> Weapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItem> Armor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItem> Helmet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItem> Ring;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items")
	TArray<UCItem*> AllItems;
	
};
