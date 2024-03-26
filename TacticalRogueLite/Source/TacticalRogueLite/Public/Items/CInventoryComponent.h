
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
	TArray<UCItem*>GetAllItems() const;
	UFUNCTION(Category = "Items|Equipment")
	TArray<UCItem*> GetEquippedItems() const;
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItemInSlot(EItemSlots inSlot) const;
	UFUNCTION(Category = "Items|Passives")
	TArray<UCItem*>GetCharms() const { return Charms; }

	UFUNCTION(Category = "Items|Equipment")
	void EquipItem(UCItem* inItem, EItemSlots inSlot = EItemSlots::EIS_None);
	UFUNCTION(Category = "Items|Passives")
	void AddCharm(UCItem* inItem);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Equipment")
	TObjectPtr<UCItem> Boots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Equipment")
	TObjectPtr<UCItem> Weapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Equipment")
	TObjectPtr<UCItem> Armor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Equipment")
	TObjectPtr<UCItem> Helmet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Equipment")
	TObjectPtr<UCItem> Ring;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items|Passives")
	TArray<UCItem*> Charms;
	
};
