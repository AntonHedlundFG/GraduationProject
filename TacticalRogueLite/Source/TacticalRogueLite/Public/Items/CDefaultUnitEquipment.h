
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CDefaultUnitEquipment.generated.h"

class ACUnit;
class UCItemData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCDefaultUnitEquipment : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Default Equipment")
	TArray<UCItemData*> DefaultEquipment;

	UFUNCTION(BlueprintCallable, Category = "Default Equipment")
	void EquipUnit(ACUnit* unit);
};
