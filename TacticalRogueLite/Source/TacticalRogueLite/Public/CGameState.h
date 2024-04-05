// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GridContent/CUnit.h"
#include "CGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnOrderUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionListUpdate);

class ACGrid;
class UCAction;
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

	UFUNCTION(BlueprintCallable)
	void AddUnitToOrder(ACUnit* inUnit) { TurnOrder.Add(inUnit); }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_TurnOrder, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;

	UFUNCTION()
	void OnRep_TurnOrder() { OnTurnOrderUpdate.Broadcast(); }

	UPROPERTY(BlueprintAssignable, Category = "Turn Order");
	FOnTurnOrderUpdate OnTurnOrderUpdate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Random")
	class UCRandomComponent* Random;

	UPROPERTY(BlueprintReadOnly, Category = "Grid|Grid")
	TObjectPtr<ACGrid> GameGrid;

	// List of all executed actions this turn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ActionList, Category = "Actions")
	TArray<UCAction*> ActionList;

	// History of all executed actions of previous turns
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Actions")
	TArray<UCAction*> ActionHistory;

	UPROPERTY(BlueprintAssignable)
	FOnActionListUpdate OnActionListUpdate;

protected:

	UFUNCTION()
	void OnRep_ActionList() { OnActionListUpdate.Broadcast(); }

};
