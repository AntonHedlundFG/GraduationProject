#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Items/CNamesAndItemsList.h"
#include "GameplayTagContainer.h"
#include "CSaveGame.generated.h"


UCLASS()
class UCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCSaveGame();
	// --- Random Component State --- //
	UPROPERTY(VisibleAnywhere, Category = "RandomComponentState")
	FRandomStream SavedRandomStream;
	
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

	// --- Character Select Data --- //
	UPROPERTY(VisibleAnywhere, Category = "UnitAndItemData")
	TArray<FCNamesAndItemsList> NamesAndItems;

	UPROPERTY(VisibleAnywhere, Category = "UnitAndItemData")
	TArray<int> ControllingPlayers;

	//All unlocked achievements, used by the Achievement subsystem.
	UPROPERTY()
	FGameplayTagContainer UnlockedAchievements;

};
