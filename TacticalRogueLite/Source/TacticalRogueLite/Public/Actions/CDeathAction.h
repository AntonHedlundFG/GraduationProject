// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CDeathAction.generated.h"

class ACUnit;

UCLASS()
class TACTICALROGUELITE_API UCDeathAction : public UCAction
{
	GENERATED_BODY()
	
public:

	virtual void StartAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	ACUnit* AffectedUnit;

protected:

	UPROPERTY()
	ACGridTile* DeathTile;

	UPROPERTY()
	int DeathTurnOrderIndex;
};
