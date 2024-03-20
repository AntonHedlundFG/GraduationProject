
#include "Grid/CGrid.h"
#include "Grid/CGridTile.h"

ACGrid::ACGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACGrid::BeginPlay()
{
	Super::BeginPlay();

	GenerateGrid();
}

void ACGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACGrid::GenerateGrid()
{
	const FVector BottomLeft = FindBottomLeftCorner();
	
	for(int x = 0 ; x < GridDimensions.X; x++)
	{
		for(int y = 0 ; y < GridDimensions.Y; y++)
		{
			FVector NodePosition = BottomLeft;
			NodePosition.X += x*NodeInterval;
			NodePosition.Y += y*NodeInterval;
			TilesPositions.Add(NodePosition);
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
