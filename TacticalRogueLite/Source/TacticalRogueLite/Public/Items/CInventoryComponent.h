
#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(Category = "Items|Equipment")
	TArray<UCItem*> GetAllEquipment() const;
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItemInSlot(EItemSlots inSlot) const;
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItem_Boots() const { return Boots; }
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItem_Weapon() const { return Weapon; }
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItem_Armor() const { return Armor; }
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItem_Helmet() const { return Helmet; }
	UFUNCTION(Category = "Items|Equipment")
	UCItem* GetItem_Ring() const { return Ring; }

	UFUNCTION(Category = "Items|Equipment")
	void EquipItem(UCItem* inItem, EItemSlots inSlot);
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
