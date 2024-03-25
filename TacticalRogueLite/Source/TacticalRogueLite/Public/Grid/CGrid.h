
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrid.generated.h"

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
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Grid Properties")
	TArray<TObjectPtr<ACGridTile>> Tiles;

	UFUNCTION()
	void GenerateTiles(int inRows, int inColumns);
	UFUNCTION()
	TArray<ACGridTile*> GetHeroSpawnTiles() const { return HeroSpawnTiles; }
	UFUNCTION()
	TArray<ACGridTile*> GetEnemySpawnTiles() const { return EnemySpawnTiles; }
	UFUNCTION()
	ACGridTile* GetTileAtPosition(int inX, int inY);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> HeroSpawnTiles;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> EnemySpawnTiles;
	
	FVector FindBottomLeftCorner() const;

	//Temporary Solution
	void GenerateSpawnTiles();

};
