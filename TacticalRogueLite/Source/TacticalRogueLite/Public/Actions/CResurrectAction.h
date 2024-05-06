// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CResurrectAction.generated.h"

class ACUnit;
class ACGridTile;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCResurrectAction : public UCAction
{
	GENERATED_BODY()

public:

	virtual void StartAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ACUnit* AffectedUnit;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ACGridTile* ResurrectOnTile;

	UPROPERTY(Replicated)
	ACGridTile* PreviousTile;

	UPROPERTY(Replicated)
	int PreviousHealth;

protected:

	//Should ResurrectPercentageHealth be used over ResurrectFlatHealth
	// to determine minimum health after resurrection?
	UPROPERTY(EditDefaultsOnly, Category = "Resurrection")
	bool bUsePercentage;

	//Minimum health as percentage of MaxHealth after resurrection
	UPROPERTY(EditDefaultsOnly, Category = "Resurrection", meta = (ClampMin = 0.0f, ClampMax = 1.0f, EditCondition = "bUsePercentage"))
	float ResurrectPercentageHealth = 0.5f;

	//Minimum health as flat value after resurrection
	UPROPERTY(EditDefaultsOnly, Category = "Resurrection", meta = (ClampMin = 1, ClampMax = 5, EditCondition = "!bUsePercentage"))
	int ResurrectFlatHealth = 1;
	
};
