

#include "Grid/CGridRoom.h"

#include "CGameMode.h"
#include "CGameState.h"
#include "Grid/CGrid.h"
#include "Utility/CRandomComponent.h"
#include "Grid/CGridTile.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Achievements/CVictoryCondition.h"
#include "Achievements/VictoryConditions/CVictoryCondition_KillEnemies.h"
#include "Achievements/VictoryConditions/CVictoryCondition_PickUpKey.h"
#include "Utility/Logging/CLogManager.h"


ACGridRoom::ACGridRoom()
{
	
}

void ACGridRoom::BeginPlay()
{
	Super::BeginPlay();
	ModeRef = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
	StateRef = Cast<ACGameState>(GetWorld()->GetGameState());
	if(!ModeRef)
	{
		LOG_WARNING("No Game Mode Reference in Spawned Room");
	}
	
	if(StateRef)
	{
		RandomComp = StateRef->Random;
		if (!RandomComp)
		{
			LOG_WARNING("Couldn't access random component from Spawned Room");
		}
	}
	else
	{
		LOG_WARNING("No State Reference in Spawned Room");
	}
}

void ACGridRoom::Initialize(ACGrid* inParentGrid, int inEnemyCount, EVictoryConditions inWinCon)
{
	GameGrid = inParentGrid;

	EnemyCount = inEnemyCount > 0 ? inEnemyCount : EnemyCount;

	if (!StateRef)
	{
		StateRef == GetWorld()->GetGameState();
		
		if (StateRef)
		{
			RandomComp = StateRef->Random;
		}
	}

	if (inWinCon == EVictoryConditions::EVC_PickUpKeys && RandomComp)
	{
		RoomWinCon = inWinCon;
		const int Keys = RandomComp->GetRandRange(1, MaxKeyCount, false);
		KeyCount = Keys;
	}
	else
	{
		RoomWinCon = EVictoryConditions::EVC_KillEnemies;
	}
}

void ACGridRoom::SetCustomPlatformDimensions(int inPlatformWidth, int inPlatformLength)
{
	PlatformWidth = inPlatformWidth;
	PlatformLength = inPlatformLength;
}

bool ACGridRoom::TryInitializeVictoryCondition(TArray<ACUnit*> inEnemies, TArray<ACPickUp*> inPickUps) const
{
	if (!ModeRef || !StateRef || inEnemies.Num() <= 0)
		return false;

	UCVictoryCondition_KillEnemies* KillWinCon;
	UCVictoryCondition_PickUpKey* KeyWinCon;
	
	switch (RoomWinCon)
	{
		case EVictoryConditions::EVC_KillEnemies:
			KillWinCon = NewObject<UCVictoryCondition_KillEnemies>(ModeRef, KillVictoryCondition);
			if (KillWinCon)
			{
				KillWinCon->Initialize(ModeRef, StateRef, this);
				KillWinCon->Enemies = inEnemies;
				ModeRef->SetVictoryCondition(KillWinCon);
				return true;
			}
			break;
		case EVictoryConditions::EVC_PickUpKeys:
			KeyWinCon = NewObject<UCVictoryCondition_PickUpKey>(ModeRef, KeyVictoryCondition);
			if (KeyWinCon)
			{
				KeyWinCon->Initialize(ModeRef, StateRef, this);
				KeyWinCon->Enemies = inEnemies;
				KeyWinCon->Keys = inPickUps;
				ModeRef->SetVictoryCondition(KeyWinCon);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}

TArray<ACGridTile*> ACGridRoom::CreateRoom(int inStartX, int inStartY, bool bWithHeroSpawns)
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();

	EnemySpawns.Empty();
	RoomTiles.Empty();

	if (!GameGrid)
		return OutArray;

	//Error control
	if (PlatformLength * 2 > RoomMaxLength - LengthVariance)
		RoomMaxLength += PlatformLength * 2;
	
	if (PlatformWidth * 2 > RoomMaxWidth - WidthVariance)
		RoomMaxWidth += PlatformWidth * 2;

	//Set Room Bounds
	X_Min = RandomComp->GetRandRange(inStartX - RoomMaxWidth + PlatformWidth, inStartX + PlatformWidth, false);
	X_Max = RandomComp->GetRandRange(X_Min + RoomMaxWidth - WidthVariance, X_Min + RoomMaxWidth, false);
	Y_Min = inStartY;
	Y_Max = RandomComp->GetRandRange(Y_Min + RoomMaxLength - LengthVariance, Y_Min + RoomMaxLength, false);

	MinCoords = FVector2d(X_Min, Y_Min);
	MaxCoords = FVector2d(X_Max, Y_Max);

	//Create entrance platform
	TArray<ACGridTile*> StartArea = CreatePlatform(inStartX, inStartY, true);
	OutArray.Append(StartArea);
	

	//Create exit tile
	const int32 ExitX = RandomComp->GetRandRange(X_Min + PlatformWidth, X_Max - PlatformWidth, false);
	ExitTile = GameGrid->SpawnTileAtCoord(ExitX, Y_Max + 1, ExitTileBP);
	if (ExitTile)
	{
		OutArray.Add(ExitTile);
	}
	//Create exit platform
	TArray<ACGridTile*> ExitArea = CreatePlatform(ExitX, Y_Max, false);
	OutArray.Append(ExitArea);


	const FVector2D StartCoords = FVector2d(inStartX, inStartY);
	const FVector2D TargetCoords = FVector2d(ExitX, Y_Max);

	//Generate room points and spawn paths between them
	const TArray<ACGridTile*> GeneratedPoints = GeneratePoints(RoomPoints, StartCoords, FVector2d(X_Min, Y_Min + PlatformLength), FVector2d(X_Max, Y_Max - PlatformLength));
	OutArray.Append(GeneratedPoints);
	TArray<ACGridTile*> RoomPointNeighbours;
	//Spawn neighbouring tiles around room points
	for (const auto Tile : GeneratedPoints)
	{
		RoomPointNeighbours.Append(SpawnNeighbours(Tile->GetGridCoords(), true));
	}
	OutArray.Append(RoomPointNeighbours);
	//If no points, create path from start to exit
	if (GeneratedPoints.Num() < 1)
	{
		const TArray<ACGridTile*> StartToEnd = CreatePath(StartCoords, TargetCoords, true);
		OutArray.Append(StartToEnd);
	}
	//else create paths between start, exit, and all points
	else
	{
		OutArray.Append(CreatePathsBetweenPoints(StartCoords, TargetCoords, GeneratedPoints));
	}
	

	//Create Hero Spawns
	HeroSpawns.Append(GenerateSpawnsOnPlatform(StartArea, 4));

	//Create Spawns for PickUps and Enemies
	GeneratePickUpSpawnTiles(GeneratedPoints);
	GenerateEnemySpawnTiles(RoomPointNeighbours, ExitArea);
	
	RoomTiles = OutArray;
	
	return OutArray;
}

TArray<ACGridTile*> ACGridRoom::CreatePath(FVector2d inStart, FVector2d inTarget, bool bWithNeighbours)
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();
	TArray<FVector2d> CoordsArray;

	int32 X_Start = inStart.X;
	int32 Y_Start = inStart.Y;
	const int32 X_Target = inTarget.X;
	const int32 Y_Target = inTarget.Y;

	while (X_Start != X_Target || Y_Start != Y_Target)
	{
		if (X_Start == X_Target)
		{
			IncrementTowardsTarget(Y_Start, Y_Target);
		}
		else if (Y_Start == Y_Target)
		{
			IncrementTowardsTarget(X_Start, X_Target);
		}
		else
		{
			int32 X_Diff = FMath::Abs(X_Target - X_Start);
			int32 Y_Diff = FMath::Abs(Y_Target - Y_Start);
			int32 Direction = RandomComp->GetRandRange(1, X_Diff + Y_Diff, false);

			if (X_Diff < Y_Diff)
			{
				if (Direction <= X_Diff)
				{
					IncrementTowardsTarget(X_Start, X_Target);
				}
				else
				{
					IncrementTowardsTarget(Y_Start, Y_Target);
				}
			}
			else
			{
				if (Direction <= Y_Diff)
				{
					IncrementTowardsTarget(Y_Start, Y_Target);
				}
				else
				{
					IncrementTowardsTarget(X_Start, X_Target);
				}
			}
		}

		ACGridTile* Tile = GameGrid->SpawnTileAtCoord(X_Start, Y_Start, StandardTileBP);
		if (Tile)
		{
			OutArray.Add(Tile);
		}
		else
		{
			CoordsArray.Add(FVector2d(X_Start, Y_Start));
		}
			
	}

	if (bWithNeighbours)
	{
		TArray<ACGridTile*> Neighbours;
		for (const auto Tile : OutArray)
		{
			const FVector2d TileCoords = Tile->GetGridCoords();
			Neighbours.Append(SpawnNeighbours(TileCoords, true));
		}
		for (const auto Coords : CoordsArray)
		{
			Neighbours.Append(SpawnNeighbours(Coords, true));
		}
		
		OutArray.Append(Neighbours);
	}
	
	return OutArray;
}

TArray<ACGridTile*> ACGridRoom::CreatePathsBetweenPoints(FVector2d inStart, FVector2d inTarget,
	TArray<ACGridTile*> inPointArray)
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();
	
	for (int i = 0; i < inPointArray.Num(); i++)
	{
		//Path from start to point 1
		if (i == 0)
		{
			const TArray<ACGridTile*> PointPath = CreatePath(inStart, inPointArray[i]->GetGridCoords(), true);
			OutArray.Append(PointPath);
		}
		//Path from last point to exit
		if (i == inPointArray.Num() - 1)
		{
			const TArray<ACGridTile*> PointPath = CreatePath(inPointArray[i]->GetGridCoords(), inTarget, true);
			OutArray.Append(PointPath);
		}
		//Path from current point to next
		if (i + 1 < inPointArray.Num())
		{
			const TArray<ACGridTile*> PointPath = CreatePath(inPointArray[i]->GetGridCoords(), inPointArray[i + 1]->GetGridCoords(), true);
			OutArray.Append(PointPath);
		}
		//Paths from even to even point, and odd to odd point
		if (i + 2 < inPointArray.Num())
		{
			const TArray<ACGridTile*> PointPath = CreatePath(inPointArray[i]->GetGridCoords(), inPointArray[i + 2]->GetGridCoords(), true);
			OutArray.Append(PointPath);
		}
	}

	return OutArray;
}

TArray<ACGridTile*> ACGridRoom::CreatePlatform(int inMiddleX, int inStartY, bool isEntrance) const
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();

	const int xMin = inMiddleX - (PlatformWidth / 2);
	for(int x = xMin ; x < (xMin + PlatformWidth); x++)
	{
		if (isEntrance)
		{
			for(int y = inStartY ; y < (inStartY + PlatformLength); y++)
			{
				ACGridTile* Tile = GameGrid->SpawnTileAtCoord(x, y, StandardTileBP);
				if (Tile)
				{
					OutArray.Add(Tile);
				}
			}
		}
		else
		{
			for(int y = inStartY ; (inStartY - PlatformLength) < y; y--)
			{
				ACGridTile* Tile = GameGrid->SpawnTileAtCoord(x, y, StandardTileBP);
				if (Tile)
				{
					OutArray.Add(Tile);
				}
			}
		}
	}
	
	return OutArray;
}

TArray<ACGridTile*> ACGridRoom::SpawnNeighbours(FVector2d inTileCoords, bool bIncludeDiagonals) const
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();

	TArray<FVector2D> Straights = UCGridUtilsLibrary::StraightDirections();
	for (FVector2D Direction : Straights)
	{
		const FVector2D Coords = inTileCoords + Direction;

		if (Coords.X < MinCoords.X || Coords.X > MaxCoords.X)
			continue;

		if (Coords.Y < MinCoords.Y || Coords.Y > MaxCoords.Y)
			continue;
		
		ACGridTile* Tile = GameGrid->SpawnTileAtCoord(Coords.X, Coords.Y, StandardTileBP);
		if (Tile)
		{
			OutArray.Add(Tile);
		}
	}
	
	if (bIncludeDiagonals)
	{
		TArray<FVector2D> Diagonals = UCGridUtilsLibrary::DiagonalDirections();
		for (FVector2D Direction : Diagonals)
		{
			const FVector2D Coords = inTileCoords + Direction;
			
			if (Coords.X < MinCoords.X || Coords.X > MaxCoords.X)
				continue;

			if (Coords.Y < MinCoords.Y || Coords.Y > MaxCoords.Y)
				continue;
			
			ACGridTile* Tile = GameGrid->SpawnTileAtCoord(Coords.X, Coords.Y, StandardTileBP);
			if (Tile)
			{
				OutArray.Add(Tile);
			}
		}
	}
	
	return OutArray;
}

FVector2d ACGridRoom::CreatePoint(int inPreviousX, FVector2d inMinCoords, FVector2d inMaxCoords) const
{
	FVector2d PointCoords = FVector2d();

	const int XMin = inMinCoords.X;
	const int YMin = inMinCoords.Y;
	const int XMax = inMaxCoords.X;
	const int YMax = inMaxCoords.Y;

	PointCoords.Y = RandomComp->GetRandRange(YMin + 1, YMax - 1, false);

	const int32 MidX = (XMin + XMax) / 2;
	if (inPreviousX >= MidX)
	{
		PointCoords.X = RandomComp->GetRandRange(XMin + 1, MidX - PlatformWidth, false);
	}
	else
	{
		PointCoords.X = RandomComp->GetRandRange(MidX + PlatformWidth, XMax - 1, false);
	}
	
	return PointCoords;
}

TArray<ACGridTile*> ACGridRoom::GeneratePoints(int inPointAmount, FVector2d inStartCoords, FVector2d inMinCoords, FVector2d inMaxCoords)
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();
	TArray<int> X_CoordsArray;
	X_CoordsArray.Add(inStartCoords.X);

	const int XMin = inMinCoords.X;
	const int YMin = inMinCoords.Y;
	const int XMax = inMaxCoords.X;
	const int YMax = inMaxCoords.Y;

	const int Length = YMax - YMin;

	int PointAmount = inPointAmount;
	if (Length < PointAmount)
	{
		PointAmount = Length;
		LOG_WARNING("Not enough space to generate desired amount of points, spawning: %d points", PointAmount);
	}

	TArray<int> SmallIndexArray;
	for (int i = 0; i < Length / 2; i++)
	{
		SmallIndexArray.Add(YMin + i);
	}
	TArray<int> BigIndexArray;
	for (int i = Length / 2; i < Length; i++)
	{
		BigIndexArray.Add(YMin + i);
	}
	
	for (int i = 0; i < PointAmount; i++)
	{
		int YCoords;
		int Index;
		
		if (i < PointAmount / 2)
		{
			Index = RandomComp->GetRandRange(0, SmallIndexArray.Num() - 1, false);
			YCoords = SmallIndexArray[Index];
			SmallIndexArray.RemoveAt(Index);
		}
		else
		{
			Index = RandomComp->GetRandRange(0, BigIndexArray.Num() - 1, false);
			YCoords = BigIndexArray[Index];
			BigIndexArray.RemoveAt(Index);
		}

		const FVector2d PointCoords = CreatePoint(X_CoordsArray[i], FVector2d(XMin, YCoords), FVector2d(XMax, YCoords));
		X_CoordsArray.Add(PointCoords.X);

		ACGridTile* Tile = GameGrid->SpawnTileAtCoord(PointCoords.X, PointCoords.Y, StandardTileBP);
		if (Tile)
		{
			OutArray.Add(Tile);
		}
	}
	
	return OutArray;
}

void ACGridRoom::IncrementTowardsTarget(int32& inValue, int32 inTarget)
{
	if (inValue < inTarget)
	{
		inValue++;
	}
	else
	{
		inValue--;
	}
}

void ACGridRoom::GeneratePickUpSpawnTiles(TArray<ACGridTile*> inPoints)
{
	const int PointAmount = inPoints.Num();
	TArray<int> Indexes;
	for (int i = 0; i < PointAmount; i++)
	{
		Indexes.Add(i);
	}

	for (int i = 0; i < PointAmount; i++)
	{
		if (i >= KeyCount)
			break;

		const int X = RandomComp->GetRandRange(0, Indexes.Num() -1, false);
		const int PointIndex = Indexes[X];
		KeySpawns.Add(inPoints[PointIndex]);
		
		Indexes.RemoveAt(X);
	}
}

void ACGridRoom::GenerateEnemySpawnTiles(TArray<ACGridTile*> inPoints, TArray<ACGridTile*> inPlatform)
{

	const bool bIsKeyRoom = RoomWinCon == EVictoryConditions::EVC_PickUpKeys;
	
	int PointAmount = inPoints.Num();
	TArray<int> Indexes;
	for (int i = 0; i < PointAmount; i++)
	{
		Indexes.Add(i);
	}

	// if (bIsKeyRoom)
	// {
	// 	for (int i = 0; i < PointAmount; i++)
	// 	{
	// 		if (i >= KeyCount)
	// 			break;
	//
	// 		const int X = RandomComp->GetRandRange(0, Indexes.Num() -1, false);
	// 		const int PointIndex = Indexes[X];
	// 		KeySpawns.Add(inPoints[PointIndex]);
	// 	
	// 		Indexes.RemoveAt(X);
	// 	}
	//
	// 	PointAmount = Indexes.Num();
	// }
	
	for (int i = 0; i < PointAmount; i++)
	{
		if (i >= EnemyCount)
			break;

		const int X = RandomComp->GetRandRange(0, Indexes.Num() -1, false);
		const int PointIndex = Indexes[X];
		EnemySpawns.Add(inPoints[PointIndex]);
		
		Indexes.RemoveAt(X);
	}

	if (PointAmount < EnemyCount)
	{
		EnemySpawns.Append(GenerateSpawnsOnPlatform(inPlatform, EnemyCount - PointAmount));
	}
}

TArray<ACGridTile*> ACGridRoom::GenerateSpawnsOnPlatform(TArray<ACGridTile*> inPlatformTiles, int inSpawnAmount) const
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();
	const int Size = inPlatformTiles.Num();

	const int SpawnAmount = Size < inSpawnAmount ? Size : inSpawnAmount;

	for (int i = 0; i < SpawnAmount; i++)
	{
		const int Min = i * Size / SpawnAmount;
		const int Max = (i + 1) * Size / SpawnAmount - 1;
		const int Index = RandomComp->GetRandRange(Min, Max, false);

		ACGridTile* Tile = inPlatformTiles[Index];
		if (Tile)
			OutArray.Add(Tile);
	}
	
	return OutArray;
}
