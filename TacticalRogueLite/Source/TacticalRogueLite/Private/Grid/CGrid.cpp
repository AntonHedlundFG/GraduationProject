
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

void ACGrid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGrid, AllTiles);
	DOREPLIFETIME(ACGrid, AllRooms);
}

ACGridRoom* ACGrid::CreateNewRoom(int inEnemyCount)
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBPs[0], GetActorLocation(), FRotator::ZeroRotator);
	
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
		Room->Initialize(this, inEnemyCount);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(StartX,StartY);

		if (PreviousRoom)
			PreviousRoom->GetExitTile()->GenerateLinks();

		for (auto tile : RoomTiles)
		{
			tile->GenerateLinks();
		}


		AllRooms.Add(Room);
		OnNewRoomSpawned();
	}
	
	return Room;
}

ACGridRoom* ACGrid::CreateStartRoom(int inEnemyCount)
{
	TObjectPtr<ACGridRoom> Room = GetWorld()->SpawnActor<ACGridRoom>(RoomBPs[0], GetActorLocation(), FRotator::ZeroRotator);
	
	if (Room)
	{
		Room->Initialize(this, inEnemyCount);
		Room->SetCustomPlatformDimensions(6, 4);
		TArray<ACGridTile*> RoomTiles = Room->CreateRoom(0,0, true);

		for (auto tile : RoomTiles)
		{
			tile->GenerateLinks();
		}

		AllRooms.Add(Room);
		OnNewRoomSpawned();
	}

	if (!Room->GetHeroSpawnTiles().IsEmpty())
		HeroSpawnTiles = Room->GetHeroSpawnTiles();
	
	return Room;
}

ACGridTile* ACGrid::SpawnTileAtCoord(int inX, int inY, TSubclassOf<ACGridTile> TileType)
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
	if (GetWorld()->GetNetMode() < NM_Client)
	{
		for (auto Element : TileMap)
		{
			if (Element.Key == inCoords)
				return Element.Value;
		}
		return nullptr;
	}
	
	for (const auto Tile : AllTiles)
	{
		if (Tile->GetGridCoords() == inCoords)
			return Tile;
	}
	return nullptr;
	
}

TSet<FVector2D> ACGrid::GetTileNeighboursCoordinates(FVector2D inCoords, bool bIncludeDiagonals /*= false*/)
{
	TSet<FVector2D> Neighbours;
	Neighbours.Add(inCoords + FVector2d(1,0)); // Up
	Neighbours.Add(inCoords + FVector2d(-1,0)); // Down
	Neighbours.Add(inCoords + FVector2d(0,1)); // Right
	Neighbours.Add(inCoords + FVector2d(0,-1)); // Left

	if(bIncludeDiagonals)
	{
		Neighbours.Append(GetDiagonalTileNeighboursCoordinates(inCoords));
	}

	return Neighbours;	
}

TSet<FVector2D> ACGrid::GetDiagonalTileNeighboursCoordinates(FVector2D inCoords)
{
	TSet<FVector2D> Neighbours;

	Neighbours.Add(inCoords + FVector2d(1,1)); // Up Right
	Neighbours.Add(inCoords + FVector2d(-1,1)); // Down Right
	Neighbours.Add(inCoords + FVector2d(1,-1)); // Up Left
	Neighbours.Add(inCoords + FVector2d(-1,-1)); // Down Left

	return Neighbours;	
}

void ACGrid::GetLevelBounds(FVector2D& outXBounds, FVector2D& outYBounds)
{
	int XMin = INT_MAX;
	int XMax = INT_MIN;
	int YMin = INT_MAX;
	int YMax = INT_MIN;

	for (const auto Room : AllRooms)
	{
		const int RoomXMin = Room->GetRoomXBounds().X;
		const int RoomXMax = Room->GetRoomXBounds().Y;
		const int RoomYMin = Room->GetRoomYBounds().X;
		const int RoomYMax = Room->GetRoomYBounds().Y;
		
		if (RoomXMin < XMin)
			XMin = RoomXMin;
		if (RoomXMax > XMax)
			XMax = RoomXMax;

		if (RoomYMin < YMin)
			YMin = RoomYMin;
		if (RoomYMax > YMax)
			YMax = RoomYMax;
	}

	//X and Y are flipped because the grid uses a conventional XY coordinate system (X = right, Y = forward/up)
	//While the camera has X as forward and Y as right (Unreal standard).
	outXBounds = FVector2D(YMin, YMax) * NodeInterval;
	outYBounds = FVector2D(XMin, XMax) * NodeInterval;
}