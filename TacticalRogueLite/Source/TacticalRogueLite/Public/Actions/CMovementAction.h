// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CMovementAction.generated.h"

class ACGridTile;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCMovementAction : public UCTargetableAction
{
	GENERATED_BODY()
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void UndoAction_Implementation(AActor* Instigator) override;
	virtual TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* inTile) override;

protected:

	ACGridTile* FromTile;
	ACUnit* MovedUnit;

};
