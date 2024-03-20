
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrid.generated.h"

class ACUnitSpawner;
class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API ACGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACGridTile> TileBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int NodeInterval = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector2D GridDimensions = FVector2D(10,10);
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> Tiles;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> HeroSpawnTiles;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> EnemySpawnTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACUnitSpawner> Spawner;

	UFUNCTION()
	void GenerateGrid(int inRows, int inColumns, int inNodeInterval);
	UFUNCTION()
	TArray<ACGridTile*> GetHeroSpawnTiles() const { return HeroSpawnTiles; }
	UFUNCTION()
	TArray<ACGridTile*> GetEnemySpawnTiles() const { return EnemySpawnTiles; }
	UFUNCTION()
	ACGridTile* GetTileAtPosition(int inX, int inY);
	
protected:
	virtual void BeginPlay() override;

private:
	FVector FindBottomLeftCorner() const;

	//Temporary Solutions
	void GenerateSpawnTiles();
	void CreateSpawner();

};
