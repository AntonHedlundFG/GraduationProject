
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridLink.generated.h"

class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API UCGridLink : public UObject
{
	GENERATED_BODY()
	
public:	
	UCGridLink();
	
	UPROPERTY()
	TObjectPtr<ACGridTile> StartTile;
	UPROPERTY()
	TObjectPtr<ACGridTile> TargetTile;

	UFUNCTION()
	void Initialize(ACGridTile* inStart, ACGridTile* inTarget);
};
