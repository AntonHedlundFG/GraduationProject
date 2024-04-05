
#pragma once

#include "CoreMinimal.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlineGameMode.h"
#include "CCharacterSelectGameMode.generated.h"

class UCStartCharacterData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACCharacterSelectGameMode : public AOnlineGameMode
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Characters")
	TObjectPtr<UCStartCharacterData> StartCharacters;
	
};
