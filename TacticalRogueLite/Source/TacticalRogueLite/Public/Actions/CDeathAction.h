// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CDeathAction.generated.h"

class ACUnit;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCDeathAction : public UCAction
{
	GENERATED_BODY()
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void UndoAction_Implementation(AActor* Instigator) override;

protected:
	
	UPROPERTY(BlueprintReadWrite)
	ACUnit* AffectedUnit;

	UPROPERTY()
	ACGridTile* DeathTile;

	UPROPERTY()
	int DeathTurnOrderIndex;
};
