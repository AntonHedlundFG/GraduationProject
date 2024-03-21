#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

UCLASS()
class UCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// --- Random Component State --- //
	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	int64 SavedTicks;

	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	int32 SavedTicksSinceSave;

	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	int32 SavedTicksAtSave;

	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	int32 SavedStartSeed;

	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	int32 SavedCurrentStateSeed;

};
