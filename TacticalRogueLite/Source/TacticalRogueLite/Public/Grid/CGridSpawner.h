
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridSpawner.generated.h"

class ACGrid;
class ACGridTile;
class ACUnit;

UCLASS()
class TACTICALROGUELITE_API ACGridSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TArray<TSubclassOf<ACUnit>> HeroUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Units")
	TArray<TSubclassOf<ACUnit>> EnemyUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings|Grid")
	TSubclassOf<ACGrid> Grid;

	TArray<FString> HeroNames = {"Hero_1", "Hero_2", "Hero_3", "Hero_4"};
	TArray<FString> EnemyNames = {"Enemy_1", "Enemy_2", "Enemy_3", "Enemy_4"};

	///Spawns units on tiles given by array
	///Tries to name units using array of strings, pass empty array to skip naming
	UFUNCTION()
	TArray<ACUnit*> SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames);
	
	///Spawns a unit on a given tile. Tries to name the unit, passing an empty string skips naming. 
	UFUNCTION()
	ACUnit* SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, FString inName);
	
	///Spawns a grid actor at a given position, and with the given dimensions
	UFUNCTION()
	ACGrid* SpawnGrid(FVector inGridCenter, int inRows, int inColumns);
	
};
