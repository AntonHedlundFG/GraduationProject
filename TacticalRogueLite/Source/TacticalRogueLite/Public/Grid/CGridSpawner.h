
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Utility/SaveGame/CSavable.h"
#include "CGridSpawner.generated.h"

class ACGameState;
class ACGameMode;
class UCVictoryCondition;
class ACGridRoom;
class ACGrid;
class ACGridTile;
class ACUnit;
class UCItemData;
class UCAllEnemiesData;
struct FGameplayTag;
struct FCUnitSpawnDetails;


UCLASS()
class TACTICALROGUELITE_API ACGridSpawner : public AActor, public ICSavable
{
	GENERATED_BODY()
	
public:	
	ACGridSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TArray<TSubclassOf<ACUnit>> HeroUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TSubclassOf<ACUnit> EnemyUnit_BP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TObjectPtr<UCAllEnemiesData> AllEnemyData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Grid")
	TSubclassOf<ACGrid> Grid;
	
	TArray<FString> HeroNames = {"Hero_1", "Hero_2", "Hero_3", "Hero_4"};
	TArray<FString> EnemyNames = {"Enemy_1", "Enemy_2", "Enemy_3", "Enemy_4"};

	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TArray<FCUnitSpawnDetails> SpawnData;
	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TArray<int> ControllingPlayers;
	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TMap<int8, FString> UnitIndexAndNames;

	///Spawns units on tiles given by array
	///Tries to name units using array of strings, pass empty array to skip naming
	UFUNCTION()
	TArray<ACUnit*> SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames, FGameplayTag inTeamTag, FGameplayTagContainer inClassTag);
	
	///Spawns a unit on a given tile.
	UFUNCTION()
	ACUnit* SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, FGameplayTag inTeamTag, FGameplayTagContainer inClassTag);

	///Spawns a unit on a given tile, also tries to name the unit and add items to it.
	UFUNCTION()
	ACUnit* SpawnAndInitializeUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, FCUnitSpawnDetails SpawnDetails, FGameplayTag inTeamTag, FGameplayTagContainer inClassTag);
	
	///Spawns a grid actor at a given position
	UFUNCTION()
	ACGrid* SpawnGrid(FVector inGridCenter) const;
	
	///Spawns a room with enemies
	UFUNCTION()
	void SpawnRoomWithEnemies(ACGrid* inGrid, int inRoomLevel = 1, int inEnemyCount = 0, bool bIsStartRoom = false);

	virtual void OnSave() override;
	virtual void OnLoad() override;

protected:
	TObjectPtr<ACGameMode> GameModeRef;
	TObjectPtr<ACGameState> GameStateRef;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
