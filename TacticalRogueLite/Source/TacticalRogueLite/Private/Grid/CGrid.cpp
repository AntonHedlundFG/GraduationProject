
#include "Grid/CGrid.h"
#include "Grid/CGridTile.h"
#include "Grid/CUnitSpawner.h"

ACGrid::ACGrid()
{
}

void ACGrid::BeginPlay()
{
	Super::BeginPlay();

	GenerateGrid(GridDimensions.X, GridDimensions.Y, NodeInterval);
	GenerateSpawnTiles();

	CreateSpawner();
}

void ACGrid::GenerateGrid(int inRows, int inColumns, int inNodeInterval)
{
	const FVector BottomLeft = FindBottomLeftCorner();
	
	for(int x = 0 ; x < inRows; x++)
	{
		for(int y = 0 ; y < inColumns; y++)
		{
			FVector NodePosition = BottomLeft;
			NodePosition.X += x*inNodeInterval;
			NodePosition.Y += y*inNodeInterval;
			TObjectPtr<ACGridTile> Tile = GetWorld()->SpawnActor<ACGridTile>(TileBlueprint, NodePosition, FRotator::ZeroRotator);
			Tile->Initialize(this, FVector2D(x,y));
			Tiles.Add(Tile);
		}
	}

	for (auto tile : Tiles)
	{
		tile->CreateLinks();
	}
}

ACGridTile* ACGrid::GetTileAtPosition(int inX, int inY)
{
	FVector2D PositionVector = FVector2D(inX, inY);
	for (auto tile : Tiles)
	{
		if (tile->GetGridCoords() == PositionVector)
		{
			return tile;
		}
	}

	return nullptr;
}

FVector ACGrid::FindBottomLeftCorner() const
{
	FVector Corner = GetActorLocation();

	Corner.X -= GridDimensions.X / 2 * NodeInterval;
	Corner.Y -= GridDimensions.Y / 2 * NodeInterval;
	
	return Corner;
}


//
//SUPER UGLY, DON'T LOOK!! WILL CHANGE LATER!
//
void ACGrid::GenerateSpawnTiles()
{
	TArray<FVector2D> HeroSpawns;
	HeroSpawns.Add(FVector2D(0,1));
	HeroSpawns.Add(FVector2D(0,3));
	HeroSpawns.Add(FVector2D(0,6));
	HeroSpawns.Add(FVector2D(0,8));

	TArray<FVector2D> EnemySpawns;
	EnemySpawns.Add(FVector2D(9,1));
	EnemySpawns.Add(FVector2D(9,3));
	EnemySpawns.Add(FVector2D(9,6));
	EnemySpawns.Add(FVector2D(9,8));
	
	
	for (auto tile : Tiles)
	{
		for (auto coords : HeroSpawns)
		{
			if (tile->GetGridCoords() == coords)
			{
				HeroSpawnTiles.Add(tile);
			}
		}

		for (auto coords : EnemySpawns)
		{
			if (tile->GetGridCoords() == coords)
			{
				EnemySpawnTiles.Add(tile);
			}
		}
	}
}

void ACGrid::CreateSpawner()
{
	TObjectPtr<ACUnitSpawner> spawner = GetWorld()->SpawnActor<ACUnitSpawner>(Spawner, GetActorLocation(), FRotator::ZeroRotator);
	spawner->SpawnUnitsFromArray(spawner->HeroUnits, HeroSpawnTiles);
	spawner->SpawnUnitsFromArray(spawner->EnemyUnits, EnemySpawnTiles);
}
