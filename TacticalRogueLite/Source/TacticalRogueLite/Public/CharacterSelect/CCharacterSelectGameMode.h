
#pragma once

#include "CoreMinimal.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlineGameMode.h"
#include "CCharacterSelectGameMode.generated.h"


class ACCharacterSelectGameState;
class UCStartCharacterData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectGameMode : public AOnlineGameMode
{
	GENERATED_BODY()

public:
	ACCharacterSelectGameMode();
	
	UPROPERTY(EditAnywhere, Category = "Player Count")
	int8 DefaultPlayerCount = 2;

	UFUNCTION(BlueprintPure)
	int GetPlayerCount() const { return PlayerCount; }

	UFUNCTION(BlueprintPure)
	ACCharacterSelectGameState* GetStateRef() { return StateRef;}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACCharacterSelectGameState* StateRef;
	
	UPROPERTY()
	int8 PlayerCount;
};
