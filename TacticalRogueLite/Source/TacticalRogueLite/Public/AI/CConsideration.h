#pragma once

#include "CoreMinimal.h"
#include "CAIContext.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CConsideration.generated.h"

UCLASS(BlueprintType)
class UCConsideration : public UDataAsset
{
	GENERATED_BODY()
public:
	// Returns the score of this consideration
	UFUNCTION(BlueprintCallable)
	virtual float Evaluate(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile, FCAIContext& Context) const { return 0; }

	UFUNCTION(BlueprintCallable)
	virtual FString ToString() const { return Name; } 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly)
	FRuntimeFloatCurve Curve;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;
	
};
