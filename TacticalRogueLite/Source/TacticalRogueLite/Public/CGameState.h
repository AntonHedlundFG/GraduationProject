// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGameState.generated.h"

class ACUnit;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Order")
	TArray<ACUnit*> TurnOrder;

};
