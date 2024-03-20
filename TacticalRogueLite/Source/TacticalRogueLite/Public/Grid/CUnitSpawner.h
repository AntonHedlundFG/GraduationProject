
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUnitSpawner.generated.h"

class ACGridTile;
class ACUnit;

UCLASS()
class TACTICALROGUELITE_API ACUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACUnitSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	TArray<TSubclassOf<ACUnit>> HeroUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	TArray<TSubclassOf<ACUnit>> EnemyUnits;
	
	UFUNCTION()
	TArray<ACUnit*> SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles);
	UFUNCTION()
	ACUnit* SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile);

protected:
	virtual void BeginPlay() override;
	
};
