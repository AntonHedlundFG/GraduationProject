
#pragma once

#include "CoreMinimal.h"
#include "Achievements/VictoryConditions/VictoryConditions.h"
#include "GameFramework/Actor.h"
#include "CGridRoom.generated.h"

enum class EVictoryConditions : uint8;
class UCVictoryCondition_PickUpKey;
class UCVictoryCondition_KillEnemies;
class ACPickUp;
class UCAllEnemiesData;
class ACGameMode;
class ACGameState;
class UCRandomComponent;
class ACGrid;
class ACGridTile;
class ACUnit;
class UCVictoryCondition;

UCLASS()
class TACTICALROGUELITE_API ACGridRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridRoom();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|VictoryCondition")
	EVictoryConditions RoomWinCon = EVictoryConditions::EVC_KillEnemies;
	

	//This is the function that creates the room, all the tiles, spawn points etc.
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> CreateRoom(int inStartX, int inStartY, bool bWithHeroSpawns = false);
	UFUNCTION(BlueprintCallable)
	void Initialize(ACGrid* inParentGrid, int inEnemyCount = 0, EVictoryConditions inWinCon = EVictoryConditions::EVC_KillEnemies);
	UFUNCTION(BlueprintCallable)
	void SetCustomPlatformDimensions(int inPlatformWidth, int inPlatformLength);
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetAllRoomTiles() { return RoomTiles; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetEnemySpawnTiles() { return EnemySpawns; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetHeroSpawnTiles() { return HeroSpawns; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetKeySpawnTiles() { return KeySpawns; }
	UFUNCTION(BlueprintCallable)
	int GetEnemyCount() { return EnemyCount; }
	UFUNCTION(BlueprintCallable)
	ACGridTile* GetExitTile() const { return ExitTile; }
	UFUNCTION(BlueprintCallable)
	EVictoryConditions GetVictoryCondition() const { return RoomWinCon; }
	UFUNCTION(BlueprintCallable)
	FVector2D GetRoomXBounds() { return FVector2D(X_Min, X_Max); }
	UFUNCTION(BlueprintCallable)
	FVector2D GetRoomYBounds() { return FVector2D(Y_Min, Y_Max); }
	bool TryInitializeVictoryCondition(TArray<ACUnit*> inEnemies = TArray<ACUnit*>(), TArray<ACPickUp*> inPickUps = TArray<ACPickUp*>()) const;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	TSubclassOf<ACGridTile> StandardTileBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	TSubclassOf<ACGridTile> ExitTileBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Units")
	TObjectPtr<UCAllEnemiesData> AllEnemyData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory Condition")
	TSubclassOf<UCVictoryCondition_KillEnemies> KillVictoryCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory Condition")
	TSubclassOf<UCVictoryCondition_PickUpKey> KeyVictoryCondition;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Spawns")
	int EnemyCount = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings|Spawns")
	int MaxKeyCount = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Room Settings|Spawns")
	int KeyCount = 0;

	//Room bounds
	UPROPERTY(BlueprintReadOnly, Category = "Room Settings|Bounds")
	int32 X_Min;
	UPROPERTY(BlueprintReadOnly, Category = "Room Settings|Bounds")
	int32 X_Max;
	UPROPERTY(BlueprintReadOnly, Category = "Room Settings|Bounds")
	int32 Y_Min;
	UPROPERTY(BlueprintReadOnly, Category = "Room Settings|Bounds")
	int32 Y_Max;

	UPROPERTY()
	FVector2D MinCoords;
	UPROPERTY()
	FVector2D MaxCoords;

	//Tiles
	UPROPERTY(VisibleAnywhere, Category = "Room Tiles")
	TObjectPtr<ACGridTile> ExitTile;
	UPROPERTY(VisibleAnywhere, Category = "Room Tiles")
	TArray<ACGridTile*> RoomTiles;
	UPROPERTY(VisibleAnywhere, Category = "Room Tiles")
	TArray<ACGridTile*> EnemySpawns;
	UPROPERTY(VisibleAnywhere, Category = "Room Tiles")
	TArray<ACGridTile*> HeroSpawns;
	UPROPERTY(VisibleAnywhere, Category = "Room Tiles")
	TArray<ACGridTile*> KeySpawns;

	///References
	UPROPERTY()
	TObjectPtr<ACGrid> GameGrid;
	UPROPERTY()
	TObjectPtr<UCRandomComponent> RandomComp;
	UPROPERTY()
	TObjectPtr<ACGameState> StateRef;
	UPROPERTY()
	TObjectPtr<ACGameMode> ModeRef;
	
	virtual void BeginPlay() override;

	///Creates paths of grid tiles between room points
	TArray<ACGridTile*> CreatePathsBetweenPoints(FVector2d inStart, FVector2d inTarget, TArray<ACGridTile*> inPointArray);
	TArray<ACGridTile*> CreatePath(FVector2d inStart, FVector2d inTarget, bool bWithNeighbours);

	///Creates points in the room for paths to connect, also used for enemy spawns
	TArray<ACGridTile*> GeneratePoints(int inPointAmount, FVector2d inStartCoords, FVector2d inMinCoords, FVector2d inMaxCoords);
	FVector2d CreatePoint(int inPreviousX, FVector2d inMinCoords, FVector2d inMaxCoords) const;

	///Used to create entrance and exit platforms in the room
	TArray<ACGridTile*> CreatePlatform(int inMiddleX, int inStartY, bool isEntrance) const;
	///Tries to spawn neighbouring tiles
	TArray<ACGridTile*> SpawnNeighbours(FVector2d inTileCoords, bool bIncludeDiagonals) const;
	void IncrementTowardsTarget(int32& inValue, int32 inTarget);

	//Generates spawn points for pickups
	void GeneratePickUpSpawnTiles(TArray<ACGridTile*> inPoints);
	
	///Tries to spawn enemies on room points, else spawns on exit platform
	void GenerateEnemySpawnTiles(TArray<ACGridTile*> inPoints, TArray<ACGridTile*> inPlatform);
	TArray<ACGridTile*> GenerateSpawnsOnPlatform(TArray<ACGridTile*> inPlatformTiles, int inSpawnAmount) const;

};
