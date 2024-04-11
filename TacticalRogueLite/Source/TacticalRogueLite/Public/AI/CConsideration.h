#pragma once

#include "CoreMinimal.h"
#include "CAIContext.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Utility/Logging/CLogManager.h"
#include "CConsideration.generated.h"

class ACGridTile;
struct FAbility;

UCLASS(BlueprintType)
class UCConsideration : public UDataAsset
{
	GENERATED_BODY()
public:
	// Returns the score of this consideration
	UFUNCTION(BlueprintNativeEvent)
	float Evaluate(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile, FCAIContext& Context);
	virtual float Evaluate_Implementation(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile, FCAIContext& Context);
	
	UFUNCTION(BlueprintCallable)
	virtual FString ToString() const { return Name; } 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;
	
};
