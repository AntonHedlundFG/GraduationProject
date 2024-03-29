// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlinePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API AOnlinePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void OnNetCleanup(UNetConnection* Connection) override;
	
	// This is used on manual deliberate disconnects, and prevents the ability to reconnect.
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Online Services")
	void Server_ManualDisconnect();

};
