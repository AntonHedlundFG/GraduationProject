#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame_Settings.generated.h"


UCLASS()
class UCSaveGame_Settings : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCSaveGame_Settings();

	UPROPERTY(BlueprintReadWrite)
	float GameSpeed;

};
