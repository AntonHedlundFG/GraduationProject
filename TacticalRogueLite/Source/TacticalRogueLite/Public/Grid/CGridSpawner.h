
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/SaveGame/CSavable.h"
#include "CGridSpawner.generated.h"

class ACGameMode;
class UCVictoryCondition;
class ACGridRoom;
class ACGrid;
class ACGridTile;
class ACUnit;
class UCItemData;
struct FCNamesAndItemsList;


UCLASS()
class TACTICALROGUELITE_API ACGridSpawner : public AActor, public ICSavable
{
	GENERATED_BODY()
	
public:	
	ACGridSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TArray<TSubclassOf<ACUnit>> HeroUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Grid")
	TSubclassOf<ACGrid> Grid;

	TArray<FString> HeroNames = {"Hero_1", "Hero_2", "Hero_3", "Hero_4"};
	TArray<FString> EnemyNames = {"Enemy_1", "Enemy_2", "Enemy_3", "Enemy_4"};

	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TArray<FCNamesAndItemsList> NamesAndItemList;
	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TArray<int> ControllingPlayers;
	UPROPERTY(VisibleAnywhere, Category = "SpawnData")
	TMap<int8, FString> UnitIndexAndNames;

	///Spawns units on tiles given by array
	///Tries to name units using array of strings, pass empty array to skip naming
	UFUNCTION()
	TArray<ACUnit*> SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames);
	
	///Spawns a unit on a given tile.
	UFUNCTION()
	ACUnit* SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile);

	///Spawns a unit on a given tile, also tries to name the unit and add items to it.
	UFUNCTION()
	ACUnit* SpawnAndInitializeUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, TArray<UCItemData*> inEquipment, FString inName);
	
	///Spawns a grid actor at a given position
	UFUNCTION()
	ACGrid* SpawnGrid(FVector inGridCenter) const;
	
	///Spawns a room with enemies
	UFUNCTION()
	void SpawnRoomWithEnemies(ACGrid* inGrid, bool bIsStartRoom = false);

	virtual void OnSave() override;
	virtual void OnLoad() override;

protected:
	TObjectPtr<ACGameMode> GameModeRef;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
