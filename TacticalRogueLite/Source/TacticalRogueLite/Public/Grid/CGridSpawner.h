
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
	
	UFUNCTION()
	TArray<ACUnit*> SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles);
	UFUNCTION()
	ACUnit* SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile);
	UFUNCTION()
	ACGrid* SpawnGrid(FVector inGridCenter, int inRows, int inColumns);
	
};
