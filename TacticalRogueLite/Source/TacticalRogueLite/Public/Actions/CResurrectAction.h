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

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void UndoAction_Implementation(AActor* Instigator) override;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ACUnit* AffectedUnit;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ACGridTile* ResurrectOnTile;

	UPROPERTY(Replicated)
	ACGridTile* PreviousTile;

	UPROPERTY(Replicated)
	int PreviousHealth;
	
};
