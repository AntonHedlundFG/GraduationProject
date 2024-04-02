
#pragma once

#include "CoreMinimal.h"
#include "ItemSlots.h"
#include "Components/ActorComponent.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "CInventoryComponent.generated.h"


enum class EItemSlots : uint8;
class UCItem;
class UCItemData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();
	
	UFUNCTION(Category = "Items")
	TArray<UCItemData*>GetAllItems() const { return AllItems; }
	UFUNCTION(Category = "Items|Equipment")
	TArray<UCItemData*> GetEquippedItems() const;
	UFUNCTION(Category = "Items|Equipment")
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

	EItemSlots ConvertTagToSlot(FGameplayTag tag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCItemData> TESTITEM;

	virtual void BeginPlay() override;


protected:
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Items")
	TArray<UCItemData*> AllItems;

	
	
};
