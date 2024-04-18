
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridRoom.generated.h"

class ACGameState;
class UCRandomComponent;
class ACGrid;
class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API ACGridRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridRoom();

	UPROPERTY()
	TObjectPtr<ACGrid> GameGrid;

	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> CreateRoom(int inStartX, int inStartY, bool bWithHeroSpawns = false);
	UFUNCTION(BlueprintCallable)
	void InitializeValues(ACGrid* inParentGrid, int inEnemyAmount = 4);
	UFUNCTION(BlueprintCallable)
	void SetCustomPlatformDimensions(int inPlatformWidth, int inPlatformLength);
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetEnemySpawnTiles() { return EnemySpawns; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetHeroSpawnTiles() { return HeroSpawns; }
	ACGridTile* GetExitTile() const { return ExitTile; }


protected:
	//Determines the width and length of area around entrance and exit 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Entrance and Exit")
	int PlatformWidth = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Entrance and Exit")
	int PlatformLength = 3;

	//Used to determine the bounds of the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Room Generation")
	int RoomMaxWidth = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Room Generation")
	int RoomMaxLength = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Room Generation")
	int WidthVariance = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Room Generation")
	int LengthVariance = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Room Generation")
	int RoomPoints = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Enemy Spawn")
	int EnemyAmount = 4;

	UPROPERTY()
	FVector2D MinCoords;
	UPROPERTY()
	FVector2D MaxCoords;

	UPROPERTY()
	TObjectPtr<ACGridTile> ExitTile;
	UPROPERTY()
	TArray<ACGridTile*> RoomTiles;
	UPROPERTY()
	TArray<ACGridTile*> EnemySpawns;
	UPROPERTY()
	TArray<ACGridTile*> HeroSpawns;
	UPROPERTY()
	TObjectPtr<UCRandomComponent> RandomComp;
	UPROPERTY()
	TObjectPtr<ACGameState> StateRef;
	
	virtual void BeginPlay() override;

	TArray<ACGridTile*> CreatePath(FVector2d inStart, FVector2d inTarget, bool bWithNeighbours);
	TArray<ACGridTile*> CreatePathsBetweenPoints(FVector2d inStart, FVector2d inTarget, TArray<ACGridTile*> inPointArray);
	
	FVector2d CreatePoint(int inPreviousX, FVector2d inMinCoords, FVector2d inMaxCoords) const;
	TArray<ACGridTile*> GeneratePoints(int inPointAmount, FVector2d inStartCoords, FVector2d inMinCoords, FVector2d inMaxCoords);
	
	TArray<ACGridTile*> CreatePlatform(int inMiddleX, int inStartY, bool isEntrance) const;
	TArray<ACGridTile*> SpawnNeighbours(FVector2d inTileCoords, bool bIncludeDiagonals) const;
	void IncrementTowardsTarget(int32& inValue, int32 inTarget);

	void GenerateEnemySpawns(TArray<ACGridTile*> inPoints, TArray<ACGridTile*> inPlatform);
	TArray<ACGridTile*> GenerateSpawnsOnPlatform(TArray<ACGridTile*> inPlatformTiles, int inSpawnAmount) const;

};
