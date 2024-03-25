// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandPattern/CConsequence.h"
#include "CDeathConsequence.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCDeathConsequence : public UCConsequence
{
	GENERATED_BODY()
	
public:

	TObjectPtr<class ACUnit> DyingUnit;

	virtual void ExecuteConsequence() override;
	virtual void UndoConsequence() override;
	virtual FString ToString() override;

private:

	int QueueIndex;
	FVector DeathLocation;
	TObjectPtr<class ACGameState> GameState;
	TObjectPtr<class ACGridTile> Tile;

};
