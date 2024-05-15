// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CAttackAction.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCAttackAction : public UCTargetableAction
{
	GENERATED_BODY()

	virtual void StartAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;
	virtual TArray<ACGridTile*> GetValidTargetTiles_Implementation(ACGridTile* inTile) override;

protected:
	
	UPROPERTY()
	ACUnit* AttackingUnit;
	
	UPROPERTY()
	ACUnit* TargetUnit;

	UPROPERTY(BlueprintReadWrite)
	FAttributeModifications ReturnModifications;

	//These tags describe the Movement.Directions used for the attack targeting.
	//For example, if the attack can hit diagonally, add Movement.Diagonal
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer AttackDirectionMovementTags;
	
};
