// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Achievements/VictoryConditions/CWinConWidget.h"
#include "GameFramework/GameStateBase.h"
#include "GridContent/CUnit.h"
#include "Units/CTurnIndicator.h"
#include "CGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnOrderUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionListUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameIsOver);

class ACGrid;
class UCAction;
class UCWinConWidget;
class UCActionVisualizerSystem;

UCLASS()
class TACTICALROGUELITE_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	TObjectPtr<UCActionVisualizerSystem> ActionVisualizerSystem;
	
public:
	ACGameState();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Visualization")
	float GetGameSpeed() const { return GameSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Visualization")
	void SetGameSpeed(float NewSpeed);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Random")
	class UCRandomComponent* Random;

	UPROPERTY(BlueprintReadOnly, Category = "Grid|Grid")
	TObjectPtr<ACGrid> GameGrid;

	// List of all executed actions this turn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ActionList, Category = "Actions")
	TArray<UCAction*> ActionList;

	// History of all executed actions of previous turns
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ActionList, Category = "Actions")
	TArray<UCAction*> ActionHistory;

	UPROPERTY(BlueprintAssignable)
	FOnActionListUpdate OnActionListUpdate;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:

	UFUNCTION()
	void OnRep_ActionList();

	//Logs a message on all clients if called on server. On client, only logs locally.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastLogMessage(ELogCategory Category, const FString& Message);

	// This is the speed at which the visualizer will play back actions and AI will spend thinking etc.
	// Should not be replicated. Set for each client individually.
	UPROPERTY(EditAnywhere, Category = "Gameplay", meta = (ClampMin = 0.1f, ClampMax = 10.0f))
	float GameSpeed = 1.0f;
	
#pragma region Victory Condition

public:
	UFUNCTION(BlueprintPure)
	FString GetWinConText() { return WinConText; }

	UFUNCTION()
	void SetWinConText(FString inText) { WinConText = inText; OnRep_WinConText(); }

	UFUNCTION()
	void SetGameIsOver(bool inbGameIsOver);

	UFUNCTION(BlueprintPure)
	bool IsGameOver() { return bGameIsOver; }

	UPROPERTY(BlueprintAssignable, Category = "Victory Condition")
	FOnGameIsOver OnGameIsOver;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory Condition")
	TObjectPtr<UCWinConWidget> WinConWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_WinConText, VisibleAnywhere, Category = "Victory Condition")
	FString WinConText;

	UPROPERTY(ReplicatedUsing = OnRep_bGameIsOver, VisibleAnywhere, Category = "Victory Condition")
	bool bGameIsOver;
	
	UFUNCTION(BlueprintCallable)
	void OnRep_WinConText();

	UFUNCTION()
	void OnRep_bGameIsOver() { OnGameIsOver.Broadcast(); }


#pragma endregion

#pragma region Turn Order

public:

	/// <summary>
	/// Adds a unit to the turn order
	/// </summary>
	/// <param name="inUnit">The unit to add</param>
	/// <param name="Index">Optional index to insert unit into turn order. 
	/// 0 means first in line, 
	/// -1 is the current unit - should only be used to for example 
	/// undo a death of the active unit</param>
	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	void AddUnitToOrder(ACUnit* inUnit, int32 Index = -2);

	//If the unit is the current unit, it will only be removed once the turn ends.
	//Returns the index of the removed unit. Index is -1 if it was the current unit.
	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	int32 RemoveUnitFromOrder(ACUnit* inUnit);

	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	ACUnit* GetCurrentUnit() { return CurrentUnit; }

	//Only includes current unit at front of array if bIncludeCurrentUnit = true
	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	TArray<ACUnit*> GetCurrentTurnOrder(bool bIncludeCurrentUnit = true);

	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	void ProgressToNextTurn();

	UFUNCTION()
	void ClearTurnOrder();

	//Triggers whenver a unit is removed/added from the turn order, or when the turn ends.
	UPROPERTY(BlueprintAssignable, Category = "Turn Order");
	FOnTurnOrderUpdate OnTurnOrderUpdate;

protected:

	//Not using ReplicatedUsing on this for now since TurnOrder should always change at the same time as this one.
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_TurnOrder, Category = "Turn Order")
	ACUnit* CurrentUnit;

	//Queue containing any unit in turn order who is not the CurrentUnit
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_TurnOrder, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;
	
	//Used when CurrentUnit is "RemoveUnitFromOrder"
	UPROPERTY()
	bool bDoNotAddCurrentBackIntoQueue = false;

	UFUNCTION()
	void OnRep_TurnOrder() { OnTurnOrderUpdate.Broadcast(); }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turn Order")
	TSubclassOf<ACTurnIndicator> TurnIndicator;

#pragma endregion

};
