
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

class UCAttributeComponent;
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

	// This determines who can control this unit. 0 means AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 ControllingPlayerIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsControlledBy(AController* inController);

	// -- TEMPORARY SOLUTION, SHOULD BE REPLACED BY INVENTORY SYSTEM --
	UFUNCTION(BlueprintCallable)
	UCItem* GetItemInSlot(EItemSlots inSlot);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCItem> TemporaryItemBoots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCItem> TemporaryItemWeapon;
	// ----------------------------------------------------------------

};
