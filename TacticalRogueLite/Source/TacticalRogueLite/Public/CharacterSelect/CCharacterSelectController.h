
#pragma once

#include "CoreMinimal.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerController.h"
#include "CCharacterSelectController.generated.h"

class ACCharacterSelectGameState;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectController : public AOnlinePlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_UpdateReady(int InWidgetIndex, bool InReadyStatus);
	UFUNCTION(Server, Reliable)
	void Server_UpdatePlayerIndex(int InWidgetIndex, int InPlayerIndex);
	UFUNCTION(Server, Reliable)
	void Server_UpdateCharacterIndex(int InWidgetIndex, int InCharacterIndex);

protected:
	UPROPERTY()
	TObjectPtr<ACCharacterSelectGameState> StateRef;
	
	virtual void BeginPlay() override;
	
};
