
#include "Grid/CGridTile.h"

#include "Grid/CGrid.h"
#include "Grid/CGridLink.h"
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
			TObjectPtr<UCGridLink> NeighbourLink = NewObject<UCGridLink>();
			NeighbourLink->Initialize(this, Neighbour);
			StraightLinks.Add(NeighbourLink);
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
			TObjectPtr<UCGridLink> NeighbourLink = NewObject<UCGridLink>();
			NeighbourLink->Initialize(this, Neighbour);
			DiagonalLinks.Add(NeighbourLink);
		}
	}
}

TArray<ACGridTile*> ACGridTile::GetNeighbours(bool bIncludeDiagonals)
{
	TArray<ACGridTile*> Neighbours;
	for (const auto link : StraightLinks)
	{
		Neighbours.Add(link->TargetTile);
	}

	if (bIncludeDiagonals)
	{
		for (const auto link : DiagonalLinks)
		{
			Neighbours.Add(link->TargetTile);
		}
	}

	return Neighbours;
}



