// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OnlinePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API AOnlinePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	virtual void CopyProperties(APlayerState* PlayerState) override;

	UPROPERTY()
	FString UniqueOwnerNetID;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	uint8 PlayerIndex = 0;
};
