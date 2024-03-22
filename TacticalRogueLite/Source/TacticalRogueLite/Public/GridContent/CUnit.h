
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

class UCItem;
class ACGridTile;
enum class EItemSlots : uint8;

UCLASS()
class TACTICALROGUELITE_API ACUnit : public ACGridContent
{
	GENERATED_BODY()

public:
	ACUnit();

	// This determines who can control this unit. 0 means AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 ControllingPlayerIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsControlledBy(AController* inController);

	// -- TEMPORARY SOLUTION, SHOULD BE REPLACED BY INVENTORY SYSTEM --
	UFUNCTION(BlueprintCallable)
	UCItem* GetItemInSlot(EItemSlots inSlot) 
	{
		
		return TemporaryItem.Get();
	}
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCItem> TemporaryItem;
	// ----------------------------------------------------------------

};
