// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CTargetableAction.generated.h"

class ACUnit;

UCLASS()
class TACTICALROGUELITE_API UCTargetableAction : public UCAction
{
	GENERATED_BODY()

protected:

	virtual void StartAction(AActor* Instigator) override;

	virtual void UndoAction(AActor* Instigator) override;

public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	ACGridTile* TargetTile;

	// List of modifiers to apply to attributes when the action is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeModification> ModifiersAppliedToTarget;

	UFUNCTION(BlueprintNativeEvent)
	TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* inTile);

	virtual TArray<ACGridTile*> GetValidTargetTiles_Implementation(ACGridTile* inTile)
	{
		return TArray<ACGridTile*>();
	}

	bool IsTileReachable(ACGridTile* fromTile, ACGridTile* toTile) 
	{ return GetValidTargetTiles(fromTile).Contains(toTile); }

#pragma endregion


#pragma region AngelScript overridables

protected:
	
	// UFUNCTION(BlueprintImplementableEvent)
	// TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACGridTile* inUnit);

#pragma endregion
};
