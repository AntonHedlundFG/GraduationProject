// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid/CGridUtilsLibrary.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridTile.h"

TArray<FVector2D> UCGridUtilsLibrary::StraightDirections()
{
	TArray<FVector2D> Directions;
	Directions.Add(FVector2D(0, 1));
	Directions.Add(FVector2D(0, -1));
	Directions.Add(FVector2D(-1, 0));
	Directions.Add(FVector2D(1, 0));

	return Directions;
}

TArray<FVector2D> UCGridUtilsLibrary::DiagonalDirections()
{
	TArray<FVector2D> Directions;
	Directions.Add(FVector2D(1, 1));
	Directions.Add(FVector2D(1, -1));
	Directions.Add(FVector2D(-1, 1));
	Directions.Add(FVector2D(-1, -1));

	return Directions;
}
/*
TArray<ACGridTile*> UCGridUtilsLibrary::BFS_Pathfinding(UCItem* inItem, ACGridTile* inStart, const ACGridTile* inTarget)
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

		TArray<ACGridTile*> Neighbours = inItem == nullptr ? CurrentTile->GetNeighbours() : inItem->GetReachableTiles(CurrentTile);
		for (auto neighbour : Neighbours)
		{
			if (neighbour == nullptr || ClosedSet.Contains(neighbour)) continue;
			if (!Cast<ACUnit>(neighbour->GetContent()))
			{
				OpenSet.Add(neighbour);
				ClosedSet.Add(neighbour);
				Parents.Add(neighbour, CurrentTile);
			}
		}
	}

	return TArray<ACGridTile*>();
}

TSet<ACGridTile*> UCGridUtilsLibrary::FloodFill(UCItem* inItem, ACGridTile* inStart, int Depth)
{
	TArray<ACGridTile*> OpenSet;
	TArray<ACGridTile*> NextSet;
	TSet<ACGridTile*> ClosedSet;

	OpenSet.Add(inStart);

	for (int i = 0; i < Depth; i++)
	{
		for (ACGridTile* Current : OpenSet)
		{
			TArray<ACGridTile*> Neighbours = inItem->GetReachableTiles(Current);
			for (ACGridTile* Neighbour : Neighbours)
			{
				if (!ClosedSet.Contains(Neighbour))
				{
					NextSet.Add(Neighbour);
					ClosedSet.Add(Neighbour);
				}

			}
		}

		OpenSet = NextSet;

	}

	return ClosedSet;
}
*/

TSet<ACGridTile*> UCGridUtilsLibrary::FloodFill(ACGridTile* inStart, int Depth, FGameplayTagContainer MovementMethods /* = FGameplayTagContainer()*/)
{
	//Default to regular straight movement.
	if (MovementMethods.IsEmpty())
		MovementMethods.AddTag(SharedGameplayTags::Movement_Straight);

	TArray<ACGridTile*> OpenSet;
	OpenSet.Add(inStart);
	TArray<ACGridTile*> NextOpenSet;
	TSet<ACGridTile*> ClosedSet;
	ClosedSet.Add(inStart);
	for (int i = 0; i < Depth; i++)
	{
		for (ACGridTile* CurrentTile : OpenSet)
		{
			for (ACGridTile* Neighbour : ReachableInSingleStep(MovementMethods, CurrentTile))
			{
				if (!ClosedSet.Contains(Neighbour))
				{
					ClosedSet.Add(Neighbour);
					NextOpenSet.Add(Neighbour);
				}
			}
		}
		OpenSet = NextOpenSet;
		NextOpenSet.Empty();
	}

	return ClosedSet;
}

TSet<ACGridTile*> UCGridUtilsLibrary::ReachableInSingleStep(FGameplayTagContainer MovementMethods, ACGridTile* inTile)
{
	TSet<ACGridTile*> Neighbours;
	
	if (MovementMethods.HasTag(SharedGameplayTags::Movement_Straight))
	{
		Neighbours.Append(inTile->GetNeighbours(false));
	}
	if (MovementMethods.HasTag(SharedGameplayTags::Movement_Diagonal))
	{
		Neighbours.Append(inTile->GetDiagonalLinks());
	}
	if (MovementMethods.HasTag(SharedGameplayTags::Movement_Knight))
	{
		//We do a 3-step BFS to get all tiles at a distance of 3
		TArray<ACGridTile*> OpenTiles;
		OpenTiles.Add(inTile);
		TArray<ACGridTile*> NextOpenTiles;
		TSet<ACGridTile*> ClosedTiles;
		ClosedTiles.Add(inTile);
		for (int i = 0; i < 3; i++)
		{
			NextOpenTiles.Empty();
			for (ACGridTile* CurrentTile : OpenTiles)
			{
				for (ACGridTile* Neighbour : CurrentTile->GetNeighbours())
				{
					if (ClosedTiles.Contains(Neighbour))
						continue;

					ClosedTiles.Add(Neighbour);
					NextOpenTiles.Add(Neighbour);
				}
			}
			OpenTiles = NextOpenTiles;
		}

		//Now that we have all tiles at a distance of 3, remove the ones that are in a straight line from
		//the starting point. This leaves us with the chess knight movement.
		for (ACGridTile* Tile : NextOpenTiles)
		{
			FVector2D CoordsDelta = Tile->GetGridCoords() - inTile->GetGridCoords();
			if (CoordsDelta.X != 0 && CoordsDelta.Y != 0)
				Neighbours.Add(Tile);
		}
	}

	return Neighbours;
}
