// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CTargetableAction.generated.h"

class UCActionComponent;
class ACUnit;

UCLASS()
class TACTICALROGUELITE_API UCTargetableAction : public UCAction
{
	GENERATED_BODY()

protected:

	virtual void StartAction(AActor* Instigator) override;

	virtual void UndoAction(AActor* Instigator) override;
	
	UPROPERTY(BlueprintReadWrite, meta = (EditCondition = false))
	UCActionComponent* TargetActionComponent;

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
	
	UFUNCTION()
	ETileHighlightModes GetTargetHighlightMode() const { return TargetHighlightMode; }

protected:
	
	// Highlight that will show on the tiles that can be targeted by the action.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETileHighlightModes TargetHighlightMode = ETileHighlightModes::ETHM_NONE;


#pragma region AngelScript overridables


#pragma endregion
};
