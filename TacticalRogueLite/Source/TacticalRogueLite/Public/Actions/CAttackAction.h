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
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	int DamageAmount = 1;
	
	int ActualDamage = 0;

	// // List of modifiers to apply to attributes when the action is active.
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	// FAttributeModification& Modifier;
	//
	// //Attribute tag to modify.
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	// FGameplayTag TagToModify;
	//
	// //Attribute to modify?
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	// FAttribute& OldAttributeValue;

	int OldHealth;

	// UFUNCTION()
	// void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, int NewHealth, int Delta);
};
