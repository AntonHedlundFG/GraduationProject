// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueLiteRandom.h"
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

	virtual void BeginPlay() override;
	
public:

	UFUNCTION(BlueprintCallable)
	void AddUnitToOrder(ACUnit* inUnit) { TurnOrder.Add(inUnit); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TurnOrder, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;

	UFUNCTION()
	void OnRep_TurnOrder() { OnTurnOrderUpdate.Broadcast(); }

	UPROPERTY(BlueprintAssignable, Category = "Turn Order");
	FOnTurnOrderUpdate OnTurnOrderUpdate;

	// --- Random --- //
	RogueLiteRandom Random;

	UFUNCTION(BlueprintCallable)
	int32 GetRandom(int32 Min, int32 Max);
	UFUNCTION(BlueprintCallable)
	int32 PeekRandom(int32 Min, int32 Max);
	UFUNCTION(BlueprintCallable)
	int32 RollBackRandom(int32 Ticks);
	UFUNCTION(BlueprintCallable)
	void SaveSeedState();

};
