// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GridContent/CUnit.h"
#include "CGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnOrderUpdate);

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACGameState();

	UFUNCTION(BlueprintCallable)
	void AddUnitToOrder(ACUnit* inUnit) { TurnOrder.Add(inUnit); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TurnOrder, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;

	UFUNCTION()
	void OnRep_TurnOrder() { OnTurnOrderUpdate.Broadcast(); }

	UPROPERTY(BlueprintAssignable, Category = "Turn Order");
	FOnTurnOrderUpdate OnTurnOrderUpdate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Random")
	class UCRogueLiteRandomComponent* Random;
	

};
