
#pragma once

#include "CoreMinimal.h"
#include "Achievements/CVictoryCondition.h"
#include "CVictoryCondition_PickUpKey.generated.h"

class ACPickUp;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCVictoryCondition_PickUpKey : public UCVictoryCondition
{
	GENERATED_BODY()

public:
	UCVictoryCondition_PickUpKey();
	
	virtual bool CheckVictoryCondition() override;

	UPROPERTY()
	TArray<ACPickUp*> Keys;
};
