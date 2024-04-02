// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CAttackAction.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCAttackAction : public UCTargetableAction
{
	GENERATED_BODY()

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void UndoAction_Implementation(AActor* Instigator) override;
	virtual TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* inTile) override;

protected:
	
	UPROPERTY()
	ACUnit* AttackingUnit;
	
	UPROPERTY()
	ACUnit* TargetUnit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	int DamageAmount = 1;

	int OldHealth;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, int NewHealth, int Delta);
};
