
#include "Grid/CGridTile.h"

#include "Grid/CGrid.h"
#include "Grid/CGridUtilsLibrary.h"
#include "Net/UnrealNetwork.h"

ACGridTile::ACGridTile()
{
	bReplicates = true;
}

void ACGridTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGridTile, ParentGrid);
	DOREPLIFETIME(ACGridTile, StraightLinks);
	DOREPLIFETIME(ACGridTile, DiagonalLinks);
	DOREPLIFETIME(ACGridTile, TileContent);
	DOREPLIFETIME(ACGridTile, GridCoords);
}

void ACGridTile::Initialize(ACGrid* inParentGrid, FVector2D inCoords)
{
	ParentGrid = inParentGrid;
	GridCoords = inCoords;
}

void ACGridTile::GenerateLinks()
{
	TArray<FVector2D> Straights = UCGridUtilsLibrary::StraightDirections();
	for (FVector2D Direction : Straights)
	{
		const FVector2D Coords = GridCoords + Direction;
		if (TObjectPtr<ACGridTile> Neighbour = ParentGrid->GetTileFromCoords(Coords))
		{
			StraightLinks.Add(Neighbour);
		}
	}
	
	TArray<FVector2D> Diagonals = UCGridUtilsLibrary::DiagonalDirections();
	for (FVector2D Direction : Diagonals)
	{
		const FVector2D Coords = GridCoords + Direction;
		if (TObjectPtr<ACGridTile> Neighbour = ParentGrid->GetTileFromCoords(Coords))
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



