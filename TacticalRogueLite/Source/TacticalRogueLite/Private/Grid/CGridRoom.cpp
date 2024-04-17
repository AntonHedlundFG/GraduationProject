

#include "Grid/CGridRoom.h"

#include "CGameState.h"
#include "Grid/CGrid.h"
#include "Utility/CRandomComponent.h"
#include "Grid/CGridTile.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Utility/Logging/CLogManager.h"


ACGridRoom::ACGridRoom()
{
	
}

void ACGridRoom::BeginPlay()
{
	Super::BeginPlay();
	StateRef = Cast<ACGameState>(GetWorld()->GetGameState());
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

void ACGridRoom::InitializeValues(ACGrid* inParentGrid, int inEnemyAmount)
{
	GameGrid = inParentGrid;
	EnemyAmount = inEnemyAmount;
}

void ACGridRoom::SetCustomPlatformDimensions(int inPlatformWidth, int inPlatformLength)
{
	PlatformWidth = inPlatformWidth;
	PlatformLength = inPlatformLength;
}

TArray<ACGridTile*> ACGridRoom::CreateRoom(int inStartX, int inStartY, bool bWithHeroSpawns)
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();

	EnemySpawns.Empty();

	if (!GameGrid)
		return OutArray;

	RandomComp->InitializeFromStart(SeedTest);

	//Set Room Bounds
	const int32 X_Min = RandomComp->GetRandRange(inStartX - RoomMaxWidth + PlatformWidth, inStartX + PlatformWidth, false);
	const int32 X_Max = RandomComp->GetRandRange(X_Min + RoomMaxWidth - WidthVariance, X_Min + RoomMaxWidth, false);
	const int32 Y_Min = inStartY;
	const int32 Y_Max = RandomComp->GetRandRange(Y_Min + RoomMaxLength - LengthVariance, Y_Min + RoomMaxLength, false);

	MinCoords = FVector2d(X_Min, Y_Min);
	MaxCoords = FVector2d(X_Max, Y_Max);

	//Create entrance platform
	TArray<ACGridTile*> StartArea = CreatePlatform(inStartX, inStartY, true);
	OutArray.Append(StartArea);
	

	//Create exit tile
	const int32 ExitX = RandomComp->GetRandRange(X_Min + PlatformWidth, X_Max - PlatformWidth, false);
	ACGridTile* ExitTile = GameGrid->SpawnTileAtIndex(ExitX, Y_Max + 1, GameGrid->StandardTileBP);
	if (ExitTile)
	{
		ExitTile->SetTileHighlightMode(ETileHighlightModes::ETHM_Attackable);
		OutArray.Add(ExitTile);
	}
	//Create exit platform
	TArray<ACGridTile*> ExitArea = CreatePlatform(ExitX, Y_Max, false);
	OutArray.Append(ExitArea);

	
	//Create random point 1
	const FVector2D StartCoords = FVector2d(inStartX, inStartY);
	const FVector2D Point_1 = CreatePoint(StartCoords, X_Min, X_Max, Y_Max);
	ACGridTile* PointTile_1 = GameGrid->SpawnTileAtIndex(Point_1.X, Point_1.Y, GameGrid->StandardTileBP);
	if (PointTile_1)
	{
		PointTile_1->SetTileHighlightMode(ETileHighlightModes::ETHM_Attackable);
		OutArray.Add(PointTile_1);
	}
	//Generate path from entrance to point1
	const TArray<ACGridTile*> PathArray_1 = CreatePath(StartCoords, Point_1, true);
	OutArray.Append(PathArray_1);
	const TArray<ACGridTile*> Neighbours_1 = SpawnNeighbours(Point_1, true);
	OutArray.Append(Neighbours_1);

	
	//Create random point 2
	const FVector2D Point_2 = CreatePoint(Point_1, X_Min, X_Max, Y_Max);
	ACGridTile* PointTile_2 = GameGrid->SpawnTileAtIndex(Point_2.X, Point_2.Y, GameGrid->StandardTileBP);
	if (PointTile_2)
	{
		PointTile_2->SetTileHighlightMode(ETileHighlightModes::ETHM_Attackable);
		OutArray.Add(PointTile_2);
	}
	//Generate path from point1 to point2
	const TArray<ACGridTile*> PathArray_2 = CreatePath(Point_1, Point_2, true);
	OutArray.Append(PathArray_2);
	const TArray<ACGridTile*> Neighbours_2 = SpawnNeighbours(Point_2, true);
	OutArray.Append(Neighbours_2);

	//Generate path from entrance to point2
	const TArray<ACGridTile*> PathArray_3 = CreatePath(StartCoords, Point_2, true);
	OutArray.Append(PathArray_3);

	//Generate path from point2 to exit
	const FVector2D TargetCoords = FVector2d(ExitX, Y_Max);
	const TArray<ACGridTile*> PathArray_4 = CreatePath(Point_2, TargetCoords, true);
	OutArray.Append(PathArray_4);

	//Create Hero Spawns
	if (bWithHeroSpawns)
	{
		HeroSpawns.Append(GenerateSpawnsOnPlatform(StartArea, 4));
	}
	
	//Create Enemy Spawns
	EnemySpawns.Add(PointTile_1);
	EnemySpawns.Add(PointTile_2);
	EnemySpawns.Append(GenerateSpawnsOnPlatform(ExitArea, EnemyAmount - 2));
	
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

		ACGridTile* Tile = GameGrid->SpawnTileAtIndex(X_Start, Y_Start, GameGrid->StandardTileBP);
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
				ACGridTile* Tile = GameGrid->SpawnTileAtIndex(x, y, GameGrid->StandardTileBP);
				if (Tile)
				{
					Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Reachable);
					OutArray.Add(Tile);
				}
			}
		}
		else
		{
			for(int y = inStartY ; (inStartY - PlatformLength) < y; y--)
			{
				ACGridTile* Tile = GameGrid->SpawnTileAtIndex(x, y, GameGrid->StandardTileBP);
				if (Tile)
				{
					Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Attackable);
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
		
		ACGridTile* Tile = GameGrid->SpawnTileAtIndex(Coords.X, Coords.Y, GameGrid->StandardTileBP);
		if (Tile)
		{
			Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Hovered);
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
			
			ACGridTile* Tile = GameGrid->SpawnTileAtIndex(Coords.X, Coords.Y, GameGrid->StandardTileBP);
			if (Tile)
			{
				Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Hovered);
				OutArray.Add(Tile);
			}
		}
	}
	
	return OutArray;
}

FVector2d ACGridRoom::CreatePoint(FVector2d inPreviousPoint, int inXMin, int inXMax, int inYMax) const
{
	FVector2d PointCoords = FVector2d();

	PointCoords.Y = RandomComp->GetRandRange(inPreviousPoint.Y + 1, inYMax - 1, false);

	int32 MidX = (inXMin + inXMax) / 2;
	if (inPreviousPoint.X >= MidX)
	{
		PointCoords.X = RandomComp->GetRandRange(inXMin + 1, MidX - 1, false);
	}
	else
	{
		PointCoords.X = RandomComp->GetRandRange(MidX, inXMax - 1, false);
	}
	
	return PointCoords;
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

TArray<ACGridTile*> ACGridRoom::GenerateSpawnsOnPlatform(TArray<ACGridTile*> inPlatformTiles, int inSpawnAmount) const
{
	TArray<ACGridTile*> OutArray = TArray<ACGridTile*>();
	const int Size = inPlatformTiles.Num();

	for (int i = 0; i < inSpawnAmount; i++)
	{
		const int Min = i * Size / inSpawnAmount;
		const int Max = (i + 1) * Size / inSpawnAmount - 1;
		const int Index = RandomComp->GetRandRange(Min, Max, false);

		ACGridTile* Tile = inPlatformTiles[Index];
		if (Tile)
			OutArray.Add(Tile);
	}
	
	return OutArray;
}
