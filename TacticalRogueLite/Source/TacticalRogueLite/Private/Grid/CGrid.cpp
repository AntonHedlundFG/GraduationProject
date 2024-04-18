
#include "Grid/CGrid.h"

#include "CGameState.h"
#include "Grid/CGridRoom.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CRandomComponent.h"


ACGrid::ACGrid()
{
	bReplicates = true;
}

ACGridRoom* ACGrid::CreateNewRoom(int inEnemyAmount)
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBP, GetActorLocation(), FRotator::ZeroRotator);
	
	if (Room)
	{
		int StartX = 0;
		int StartY = 0;
		ACGridRoom* PreviousRoom = GetLatestRoom();
		if (PreviousRoom)
		{
			StartX = PreviousRoom->GetExitTile()->GetGridCoords().X;
			StartY = PreviousRoom->GetExitTile()->GetGridCoords().Y + 1;
		}
		Room->InitializeValues(this, inEnemyAmount);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(StartX,StartY);

		if (PreviousRoom)
			PreviousRoom->GetExitTile()->GenerateLinks();

		for (auto tile : RoomTiles)
		{
			tile->GenerateLinks();
		}

		AllRooms.Add(Room);
	}
	
	return Room;
}

ACGridRoom* ACGrid::CreateStartRoom(int inStartX, int inStartY)
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBP, GetActorLocation(), FRotator::ZeroRotator);

	if (ACGameState* State = Cast<ACGameState>(GetWorld()->GetGameState()))
	{
		State->Random->InitializeFromStart(SeedTest);
	}
	
	if (Room)
	{
		Room->InitializeValues(this, 4);
		Room->SetCustomPlatformDimensions(6, 4);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(inStartX,inStartY, true);

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
