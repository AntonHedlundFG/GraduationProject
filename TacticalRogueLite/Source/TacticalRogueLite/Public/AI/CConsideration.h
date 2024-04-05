#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CConsideration.generated.h"

UCLASS(BlueprintType)
class UCConsideration : public UDataAsset
{
	GENERATED_BODY()
public:
	// Returns the score of this consideration
	UFUNCTION(BlueprintCallable)
	int32 GetScore() const { return 0; }

	UFUNCTION(BlueprintCallable)
	virtual FString ToString() const { return Name; } 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=100))
	int32 Weight;
};
