
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrid.generated.h"

class ACGridRoom;
class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API ACGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings|Testing")
	int SeedTest = 1993;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACGridTile> StandardTileBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACGridRoom> RoomBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int NodeInterval = 1000;
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid Properties")
	TMap<FVector2D,TObjectPtr<ACGridTile>> TileMap;

	UFUNCTION()
	ACGridRoom* CreateNewRoom(int inEnemyAmount = 4);
	UFUNCTION()
	ACGridRoom* CreateStartRoom(int inStartX, int inStartY);
	UFUNCTION()
	ACGridTile* SpawnTileAtIndex(int inX, int inY, TSubclassOf<ACGridTile> TileType);
	UFUNCTION()
	TArray<ACGridTile*> GetHeroSpawnTiles() const { return HeroSpawnTiles; }
	UFUNCTION()
	TArray<ACGridTile*> GetEnemySpawnTiles() const { return EnemySpawnTiles; }
	UFUNCTION()
	ACGridTile* GetTileFromCoords(FVector2D inCoords);
	UFUNCTION()
	TArray<ACGridTile*> GetAllTiles() { return AllTiles; };
	UFUNCTION()
	ACGridRoom* GetLatestRoom() { return AllRooms.Last(); }
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> AllTiles;
	UPROPERTY()
	TArray<TObjectPtr<ACGridRoom>> AllRooms;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> HeroSpawnTiles;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> EnemySpawnTiles;

	//Temporary Solution
	void GenerateSpawnTiles();

};
