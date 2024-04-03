
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/CItemData.h"
#include "CInventoryComponent.generated.h"

class UCItem;
class UCItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, UCInventoryComponent*, OwningComp, UCItemData*, ItemData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	TObjectPtr<UCItemData> Helmet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items|Equipment")
	TObjectPtr<UCItemData> Ring;
};


