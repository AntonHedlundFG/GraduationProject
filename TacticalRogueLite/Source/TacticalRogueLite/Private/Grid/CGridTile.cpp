
#include "Grid/CGridTile.h"

#include "Grid/CGrid.h"
#include "Grid/CGridUtils.h"
#include "Net/UnrealNetwork.h"

void ACGridTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGridTile, ParentGrid);
	DOREPLIFETIME(ACGridTile, StraightLinks);
	DOREPLIFETIME(ACGridTile, DiagonalLinks);
	DOREPLIFETIME(ACGridTile, TileContent);
	DOREPLIFETIME(ACGridTile, GridCoords);
}

ACGridTile::ACGridTile()
{
	bReplicates = true;
}

void ACGridTile::Initialize(ACGrid* inParentGrid, FVector2D inCoords)
{
	ParentGrid = inParentGrid;
	GridCoords = inCoords;
}

void ACGridTile::BeginPlay()
{
	Super::BeginPlay();
}



void ACGridTile::CreateLinks()
{
	const int TopX = ParentGrid->GridDimensions.X;
	const int TopY = ParentGrid->GridDimensions.Y;

	TArray<FVector2D> StraightDirections = CGridUtils::StraightDirections();
	for (auto direction : StraightDirections)
	{
		const FVector2D Coords = GridCoords + direction;
		if (Coords.X < 0 || Coords.Y < 0) continue;
		if (Coords.X >= TopX || Coords.Y >= TopY) continue;

		if(TObjectPtr<ACGridTile> Neighbour = ParentGrid->GetTileAtPosition(Coords.X, Coords.Y))
		{
			StraightLinks.Add(Neighbour);
		}
	}

	TArray<FVector2D> DiagonalDirections = CGridUtils::DiagonalDirections();
	for (auto direction : DiagonalDirections)
	{
		const FVector2D Coords = GridCoords + direction;
		if (Coords.X < 0 || Coords.Y < 0) continue;
		if (Coords.X >= TopX || Coords.Y >= TopY) continue;

		if(TObjectPtr<ACGridTile> Neighbour = ParentGrid->GetTileAtPosition(Coords.X, Coords.Y))
		{
			DiagonalLinks.Add(Neighbour);
		}
	}
}

TArray<ACGridTile*> ACGridTile::GetNeighbours(bool bIncludeDiagonals)
{
	TArray<ACGridTile*> Neighbours = StraightLinks;

	if (bIncludeDiagonals)
		Neighbours.Append(DiagonalLinks);

	return Neighbours;
}



