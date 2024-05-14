// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CHealAction_AOE.generated.h"

class ACUnit;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCHealAction_AOE : public UCAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Range = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanHealSelf = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	ACUnit* AttackingUnit; //TODO: Hide

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<ACUnit*> TargetsArray;

	virtual void StartAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;
	virtual TSet<ACGridTile*> GetActionInfluencedTiles_Implementation(ACGridTile* fromTile) override;

protected:

	UPROPERTY()
	TArray<int> ModifiersTargetActualDeltas;

	// List of modifiers to apply to attributes when the action is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeModification> ModifiersAppliedToTarget;

};
