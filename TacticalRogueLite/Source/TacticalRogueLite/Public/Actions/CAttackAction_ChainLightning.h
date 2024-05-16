// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CAttackAction_ChainLightning.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAttackAction_ChainLightning : public UCTargetableAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int BounceRange = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int BounceCount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer BlockingTags;

	UPROPERTY(Replicated, BlueprintReadWrite)
	ACUnit* AttackingUnit;
	UPROPERTY(Replicated, BlueprintReadWrite)
	TArray<ACUnit*> UnitsHit;

	TArray<FAttributeModifications> ReturnModifications;
	int Damaged = 0;
	
	virtual TArray<ACGridTile*> GetValidTargetTiles_Implementation(ACGridTile* inTile) override;
	virtual void StartAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;

	TArray<ACGridTile*> GetTargetsInRange(ACGridTile* inTile, int Range);
	void DamageTarget(ACUnit* Target);
};
