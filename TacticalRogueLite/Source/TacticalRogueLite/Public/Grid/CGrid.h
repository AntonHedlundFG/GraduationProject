
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACGridRoom> RoomBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int NodeInterval = 1000;
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid Properties")
	TMap<FVector2D,TObjectPtr<ACGridTile>> TileMap;

	UFUNCTION()
	ACGridRoom* CreateNewRoom();
	UFUNCTION()
	ACGridRoom* CreateStartRoom();
	UFUNCTION()
	ACGridTile* SpawnTileAtCoord(int inX, int inY, TSubclassOf<ACGridTile> TileType);
	UFUNCTION()
	TArray<ACGridTile*> GetHeroSpawnTiles() const { return HeroSpawnTiles; }
	UFUNCTION(ScriptCallable)
	ACGridTile* GetTileFromCoords(FVector2D inCoords);
	UFUNCTION()
	TArray<ACGridTile*> GetAllTiles() { return AllTiles; };
	UFUNCTION()
	ACGridRoom* GetLatestRoom() { return AllRooms.Last(); }
	UFUNCTION()
	static TSet<FVector2D> GetTileNeighboursCoordinates(FVector2D inCoords, bool bIncludeDiagonals = false);
	UFUNCTION()
	static TSet<FVector2D> GetDiagonalTileNeighboursCoordinates(FVector2D inCoords);
	
protected:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ACGridTile>> AllTiles;
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ACGridRoom>> AllRooms;
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> HeroSpawnTiles;
};
