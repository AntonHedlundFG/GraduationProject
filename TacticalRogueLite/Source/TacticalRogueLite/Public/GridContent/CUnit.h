
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

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
	
public:
	ACUnit();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	// This determines who can control this unit. 0 means AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 ControllingPlayerIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsControlledBy(AController* inController);

	// -- TEMPORARY SOLUTION, SHOULD BE REPLACED BY INVENTORY SYSTEM --
	UFUNCTION(BlueprintCallable)
	UCItem* GetItemInSlot(EItemSlots inSlot);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	TObjectPtr<UCInventoryComponent> Inventory;
	
	// ----------------------------------------------------------------


	

protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);
};
