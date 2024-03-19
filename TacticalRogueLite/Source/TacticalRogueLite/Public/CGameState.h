// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GridContent/CUnit.h"
#include "CGameState.generated.h"


/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void AddUnitToOrder(ACUnit* inUnit) { TurnOrder.Add(inUnit); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;

};
