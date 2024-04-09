
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
	UFUNCTION()
	void Server_UpdatePlayerIndex(int InWidgetIndex, int InPlayerIndex);
	UFUNCTION()
	void Server_UpdateCharacterIndex(int InWidgetIndex, int InCharacterIndex);

protected:
	UPROPERTY()
	TObjectPtr<ACCharacterSelectGameState> StateRef;
	
	virtual void BeginPlay() override;
	
};
