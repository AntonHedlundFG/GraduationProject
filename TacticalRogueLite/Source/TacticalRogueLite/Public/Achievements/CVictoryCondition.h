// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CVictoryCondition.generated.h"

class ACGameMode;
class ACGameState;
class ACGridRoom;
class ACUnit;

/** This object, or a child of it, should be created by the level generation
 * and assigned to the CGameMode. At the end of each turn, the GameMode will
 * run CheckVictoryCondition().
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCVictoryCondition : public UObject
{
	GENERATED_BODY()

public:
	UCVictoryCondition();

	void Initialize(const ACGameMode* inGameMode, const ACGameState* inGameState, const ACGridRoom* inRoom);

	FString GetConditionText() const { return ConditionText; }
	
	//This function should check the game state and determine whether the players
	//have reached the conditions required to complete the objective.
	// Returns true if victory is achieved.
	virtual bool CheckVictoryCondition();
	
	//This function should check the game state and determine whether the players
	//have failed the objective, whether through death or otherwise.
	// Returns true if the players have lost.
	virtual bool CheckLossCondition();

	bool bWonByCheating = false;
	bool bLoseByCheating = false;

	UPROPERTY()
	TArray<ACUnit*> Enemies;

protected:
	UPROPERTY()
	TSoftObjectPtr<ACGameMode> GameModeRef;
	UPROPERTY()
	TSoftObjectPtr<ACGameState> GameStateRef;
	UPROPERTY()
	TSoftObjectPtr<ACGridRoom> ParentRoom;


	
	UPROPERTY()
	FString ConditionText;
	UPROPERTY()
	int VictoryConditionLevel = 0;

	
};
