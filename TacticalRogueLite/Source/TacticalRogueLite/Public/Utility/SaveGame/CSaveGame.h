#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GridContent/UnitDataAssets/CUnitSpawnDetails.h"
#include "GameplayTagContainer.h"
#include "CSaveGame.generated.h"


UCLASS()
class UCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCSaveGame();
	// --- Random Component State --- //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	FRandomStream SavedRandomStream;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	int64 SavedTicks;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	int32 SavedTicksSinceSave;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	int32 SavedTicksAtSave;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	int32 SavedStartSeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RandomComponentState")
	int32 SavedCurrentStateSeed;

	// --- Character And Player Data --- //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 PlayerCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UnitAndItemData")
	TArray<FCUnitSpawnDetails> UnitDetails;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UnitAndItemData")
	TArray<int> ControllingPlayers;

	//All unlocked achievements, used by the Achievement subsystem.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGameplayTagContainer UnlockedAchievements;

};
