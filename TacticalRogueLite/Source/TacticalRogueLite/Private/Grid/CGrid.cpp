
#include "Grid/CGrid.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"


ACGrid::ACGrid()
{
	bReplicates = true;
}

void ACGrid::GenerateTiles(int inRows, int inColumns)
{
	const FVector BottomLeft = FindBottomLeftCorner();
	
	for(int x = 0 ; x < inRows; x++)
	{
		for(int y = 0 ; y < inColumns; y++)
		{
			FVector NodePosition = BottomLeft;
			NodePosition.X += x*NodeInterval;
			NodePosition.Y += y*NodeInterval;
			FVector2D TileCoords = FVector2D(x,y);
			TObjectPtr<ACGridTile> Tile = GetWorld()->SpawnActor<ACGridTile>(StandardTileBlueprint, NodePosition, FRotator::ZeroRotator);
			Tile->Initialize(this, TileCoords);
			TileMap.Add(TileCoords, Tile);
			AllTiles.Add(Tile);
		}
	}
	
	for (auto tile : TileMap)
	{
		tile.Value->GenerateLinks();
	}

	GenerateSpawnTiles();
}
	
ACGridTile* ACGrid::GetTileFromCoords(FVector2D inCoords)
{
	for (auto Element : TileMap)
	{
		if (Element.Key == inCoords)
			return Element.Value;
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
	
	
	for (auto tile : TileMap)
	{
		for (auto coords : HeroSpawns)
		{
			if (tile.Key == coords)
			{
				HeroSpawnTiles.Add(tile.Value);
			}
		}

		for (auto coords : EnemySpawns)
		{
			if (tile.Key == coords)
			{
				EnemySpawnTiles.Add(tile.Value);
			}
		}
	}
}
