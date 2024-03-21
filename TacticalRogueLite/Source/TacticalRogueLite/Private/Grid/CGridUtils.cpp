#include "Grid/CGridUtils.h"

#include "GridContent/CUnit.h"
#include "Items/CItem.h"

TArray<FVector2D> CGridUtils::StraightDirections()
{
	TArray<FVector2D> Directions;
	Directions.Add(FVector2D(0,1));
	Directions.Add(FVector2D(0,-1));
	Directions.Add(FVector2D(-1,0));
	Directions.Add(FVector2D(1,0));
	
	return Directions;
}

TArray<FVector2D> CGridUtils::DiagonalDirections()
{
	TArray<FVector2D> Directions;
	Directions.Add(FVector2D(1,1));
	Directions.Add(FVector2D(1,-1));
	Directions.Add(FVector2D(-1,1));
	Directions.Add(FVector2D(-1,-1));
		
	return Directions;
}

TArray<ACGridTile*> CGridUtils::BFS_Pathfinding(UCItem* inItem, ACGridTile* inStart, const ACGridTile* inTarget)
{
	TArray<ACGridTile*> OpenSet;
	TSet<ACGridTile*> ClosedSet;
	TMap<ACGridTile*, ACGridTile*> Parents;

	OpenSet.Add(inStart);
	ClosedSet.Add(inStart);
	while (OpenSet.Num() > 0)
	{
		ACGridTile* CurrentTile = OpenSet[0];
		OpenSet.RemoveAt(0);

		if (CurrentTile == inTarget)
		{
			TArray<ACGridTile*> Path;
			ACGridTile* PathTile = CurrentTile;
			while (PathTile != nullptr)
			{
				Path.Insert(PathTile, 0);
				PathTile = Parents[PathTile];
			}
			return Path;
		}
		
		TArray<ACGridTile*> Neighbours = inItem == nullptr? CurrentTile->GetNeighbours() : inItem->GetReachableTiles(CurrentTile);
		for (auto neighbour : Neighbours)
		{
			if (neighbour == nullptr || ClosedSet.Contains(neighbour)) continue;
			if(!Cast<ACUnit>(neighbour->GetContent()))
			{
				OpenSet.Add(neighbour);
				ClosedSet.Add(neighbour);
				Parents.Add(neighbour, CurrentTile);
			}
		}
	}
	
	return TArray<ACGridTile*>();
}
