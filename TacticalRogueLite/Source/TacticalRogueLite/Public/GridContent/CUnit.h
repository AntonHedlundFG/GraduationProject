
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CUnit.generated.h"

class UCItem;
enum class EItemSlots : uint8;

UCLASS()
class TACTICALROGUELITE_API ACUnit : public ACGridContent
{
	GENERATED_BODY()

public:

	UCItem* GetItemInSlot(EItemSlots inSlot) 
	{
		//TEMPORARY SOLUTION
		return TemporaryItem.Get();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCItem> TemporaryItem;
	
};
