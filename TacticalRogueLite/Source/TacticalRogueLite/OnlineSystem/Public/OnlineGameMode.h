// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineGameMode.generated.h"

class AOnlinePlayerState;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API AOnlineGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online Services")
	bool bPlayersCanReconnectDuringMatch = true;

	TArray<AOnlinePlayerState*> InactiveStates;

	uint8 NextPlayerIndex = 1; // Host is always 0

};
