
#include "Grid/CGrid.h"

#include "Grid/CGridRoom.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"


ACGrid::ACGrid()
{
	bReplicates = true;
}

// void ACGrid::GenerateTiles(int inRows, int inColumns)
// {
// 	for(int x = 0 ; x < inRows; x++)
// 	{
// 		for(int y = 0 ; y < inColumns; y++)
// 		{
// 			SpawnTileAtIndex(x, y, StandardTileBP);
// 		}
// 	}
// 	for (auto tile : TileMap)
// 	{
// 		tile.Value->GenerateLinks();
// 	}
// 	
// 	GenerateSpawnTiles();
// }

ACGridRoom* ACGrid::CreateNewRoom(int inStartX, int inStartY)
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBP, GetActorLocation(), FRotator::ZeroRotator);
	
	if (Room)
	{
		Room->InitializeValues(this);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(inStartX,inStartY);

		for (auto tile : RoomTiles)
		{
			tile->GenerateLinks();
		}

		AllRooms.Add(Room);
	}
	
	return Room;
}

ACGridRoom* ACGrid::CreateStartRoom()
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBP, GetActorLocation(), FRotator::ZeroRotator);
	
	if (Room)
	{
		Room->InitializeValues(this, 4);
		Room->SetCustomPlatformDimensions(6, 4);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(5,0, true);

		for (auto tile : RoomTiles)
		{
			tile->GenerateLinks();
		}

		AllRooms.Add(Room);
	}

	if (!Room->GetHeroSpawnTiles().IsEmpty())
		HeroSpawnTiles = Room->GetHeroSpawnTiles();
	
	EnemySpawnTiles = Room->GetEnemySpawnTiles();
	

	// GenerateSpawnTiles();
	
	return Room;
}

ACGridTile* ACGrid::SpawnTileAtIndex(int inX, int inY, TSubclassOf<ACGridTile> TileType)
{
	FVector TilePosition = GetActorLocation();
	TilePosition.X += inY * NodeInterval;
	TilePosition.Y += inX * NodeInterval;
	TilePosition.Z = 0;

	const FVector2D TileCoords = FVector2D(inX,inY);

	if (TileMap.Contains(TileCoords))
	{
		return nullptr;
	}
	
	TObjectPtr<ACGridTile> Tile = GetWorld()->SpawnActor<ACGridTile>(TileType, TilePosition, FRotator::ZeroRotator);
	if (Tile)
	{
		Tile->Initialize(this, TileCoords);
		TileMap.Add(TileCoords, Tile);
		AllTiles.Add(Tile);
	}
	
	return Tile;
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



//
//SUPER UGLY, DON'T LOOK!! WILL CHANGE LATER!
//
void ACGrid::GenerateSpawnTiles()
{
	TArray<FVector2D> HeroSpawns;
	HeroSpawns.Add(FVector2D(2,1));
	HeroSpawns.Add(FVector2D(4,1));
	HeroSpawns.Add(FVector2D(6,1));
	HeroSpawns.Add(FVector2D(8,1));

	TArray<FVector2D> EnemySpawns;
	EnemySpawns.Add(FVector2D(2,10));
	EnemySpawns.Add(FVector2D(4,10));
	EnemySpawns.Add(FVector2D(6,10));
	EnemySpawns.Add(FVector2D(8,10));
	
	
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
