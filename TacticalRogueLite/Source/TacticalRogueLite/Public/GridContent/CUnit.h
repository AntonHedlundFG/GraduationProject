
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

class UCActionComponent;
class UCAttributeComponent;
class UCInventoryComponent;
class UCItem;
class ACGridTile;
enum class EItemSlots : uint8;

UCLASS()
class TACTICALROGUELITE_API ACUnit : public ACGridContent
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCActionComponent> ActionComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	TObjectPtr<UCInventoryComponent> InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
	FString UnitName;
	
public:
	ACUnit();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	UFUNCTION(BlueprintCallable)
	FString GetUnitName() { return UnitName.IsEmpty() ? GetName() : UnitName; }
	UFUNCTION(BlueprintCallable)
	void SetUnitName(FString inName) { UnitName = inName; }

	// This determines who can control this unit. 0 means AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 ControllingPlayerIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsControlledBy(AController* inController);
	
	UFUNCTION(BlueprintCallable)
	UCItemData* GetItemDataInSlot(FGameplayTag inSlot);
	
	UFUNCTION(BlueprintCallable)
	bool TryGetAbilityInSlot(FGameplayTag ItemSlot, FAbility& outAbility);

	UFUNCTION(BlueprintCallable)
	UCAttributeComponent* GetAttributeComp() const { return AttributeComp; }
	
	UFUNCTION(BlueprintCallable)
	UCActionComponent* GetActionComp() const { return ActionComp; }

	UFUNCTION(BlueprintCallable)
	UCInventoryComponent* GetInventoryComp() const { return InventoryComp; }

	
	// ----------------------------------------------------------------
	

};
