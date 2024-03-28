// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid/CGridUtilsLibrary.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridTile.h"
#include "Items/CItem.h"

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

TSet<ACGridTile*> UCGridUtilsLibrary::FloodFillFromTag(FGameplayTagContainer MovementMethods, ACGridTile* inStart, int Depth)
{
	TSet<ACGridTile*> TestSet;
	for (ACGridTile* Tile : inStart->GetNeighbours())
		TestSet.Add(Tile);

	//NOT IMPLEMENTED
	return TestSet;
}
