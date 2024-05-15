// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid/CGridUtilsLibrary.h"
#include "Grid/CGridTile.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Grid/CGrid.h"

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

TArray<ACGridTile*> UCGridUtilsLibrary::BFS_Pathfinding(ACGridTile* inStart, const ACGridTile* inTarget, const FGameplayTagContainer& MovementTags,	const FGameplayTagContainer& BlockingTags /*= FGameplayTagContainer()*/)
{
	TArray<ACGridTile*> OpenSet;
	TSet<ACGridTile*> ClosedSet;
	TMap<ACGridTile*, ACGridTile*> Parents;
	Parents.Add(inStart, nullptr);

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

		TSet<ACGridTile*> Neighbours = ReachableInSingleStep(CurrentTile, MovementTags, BlockingTags);
		for (auto neighbour : Neighbours)
		{
			if (neighbour == nullptr || ClosedSet.Contains(neighbour)) continue;
			if (!(neighbour->GetContent() && neighbour->GetContent()->GetGameplayTags().HasAny(BlockingTags)))
			{
				OpenSet.Add(neighbour);
				ClosedSet.Add(neighbour);
				Parents.Add(neighbour, CurrentTile);
			}
		}
	}

	return TArray<ACGridTile*>();
}

#pragma region FTilePriority
// Helper struct for A* pathfinding
struct FTilePriority
{
	ACGridTile* Tile;
	float FCost; // Total cost f = g + h

	FTilePriority(ACGridTile* InTile, float InFCost) : Tile(InTile), FCost(InFCost) {}

	// Operator overloads for priority comparison
	bool operator>(const FTilePriority& Other) const
	{
		return FCost > Other.FCost;
	}
	bool operator<(const FTilePriority& Other) const
	{
		return FCost < Other.FCost;
	}
};
#pragma endregion

TArray<ACGridTile*> UCGridUtilsLibrary::AStar_Pathfinding(ACGridTile* inStart, ACGridTile* inTarget, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags /*= FGameplayTagContainer()*/)
{
    if (!inStart || !inTarget) return {};

    TArray<FTilePriority> OpenSet;
    TMap<ACGridTile*, ACGridTile*> Parents;
    TMap<ACGridTile*, float> GCosts;
    TMap<ACGridTile*, float> FCosts;

    OpenSet.Add(FTilePriority(inStart, 0.0f));
    Parents.Add(inStart, nullptr);
    GCosts.Add(inStart, 0.0f);
    FCosts.Add(inStart, 0.0f);

    while (OpenSet.Num() > 0)
    {
        // Sort the array to find the lowest FCost, Get and remove it
        OpenSet.Sort();
        FTilePriority Current = OpenSet[0];
        OpenSet.RemoveAt(0); 

        ACGridTile* CurrentTile = Current.Tile;
    	
        if (CurrentTile == inTarget) // Construct the path if we reached the target
        {
            TArray<ACGridTile*> Path;
            while (CurrentTile != nullptr)
            {
                Path.Insert(CurrentTile, 0);
                CurrentTile = Parents[CurrentTile];
            }
            return Path; // Return the path
        }

    	// Get the neighbours with ReachableInSingleStep which returns a set of tiles based on current movement tags
        TSet<ACGridTile*> Neighbours = ReachableInSingleStep(CurrentTile, MovementTags, BlockingTags); 
        for (ACGridTile* Neighbour : Neighbours)
        {
            if (!Neighbour || Neighbour->GetContent() && Neighbour->GetContent()->GetGameplayTags().HasAny(BlockingTags))
                continue;

            float TentativeGCost = GCosts[CurrentTile] + Neighbour->GetCost();
            float HeuristicCost = GetManhattanDistance(Neighbour, inTarget); // TODO: Look into implementing a better heuristic
            float TentativeFCost = TentativeGCost + HeuristicCost;

            if (!FCosts.Contains(Neighbour) || TentativeFCost < FCosts[Neighbour])
            {
                Parents.Add(Neighbour, CurrentTile);
                GCosts.Add(Neighbour, TentativeGCost);
                FCosts.Add(Neighbour, TentativeFCost);
                OpenSet.Add(FTilePriority(Neighbour, TentativeFCost));
            }
        }
    }

    return TArray<ACGridTile*>(); // Return empty array if no path was found
}

float UCGridUtilsLibrary::GetManhattanDistance(const ACGridTile* From, const ACGridTile* To)
{
	FVector2D FromLocation = From->GetGridCoords();
	FVector2D ToLocation = To->GetGridCoords();

	// Compute Manhattan distance
	return FMath::Abs(FromLocation.X - ToLocation.X) + FMath::Abs(FromLocation.Y - ToLocation.Y);
}

ACGridContent* UCGridUtilsLibrary::GetClosestGridContent(ACGridTile* inStart, TArray<ACGridContent*>& ContentArray, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags)
{
	ACGridContent* ClosestContent = nullptr;
	int32 ClosestDistance = INT32_MAX;
	// We use a BFS to find the closest content
	for (int i = 0; i < ContentArray.Num(); ++i)
	{
		ACGridContent* Content = ContentArray[i];
		ACGridTile* ContentTile = Content->GetTile();
		TArray<ACGridTile*> Path = AStar_Pathfinding(inStart, ContentTile, MovementTags, BlockingTags);
		if (Path.Num() < ClosestDistance)
		{
			ClosestDistance = Path.Num();
			ClosestContent = Content;
		}
	}
	
	return ClosestContent;
}

int32 UCGridUtilsLibrary::GetAStarPathFindingDistance(ACGridTile* inStart, ACGridTile* inTarget,
	const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags)
{
	return AStar_Pathfinding(inStart, inTarget, MovementTags, BlockingTags).Num();
}

int32 UCGridUtilsLibrary::GetBFSPathFindingDistance(ACGridTile* inStart, ACGridTile* inTarget,
	const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags)
{
	return BFS_Pathfinding(inStart, inTarget, MovementTags, BlockingTags).Num();
}

void UCGridUtilsLibrary::GetProximityScore( ACGridTile* inStart, const TArray<ACGridTile*>& inTargets,
	const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags,
	TMap<ACGridTile*, float>& outScores)
{
	// Get distances to all targets
	for (ACGridTile* Target : inTargets)
	{
		int32 Distance = GetAStarPathFindingDistance(inStart, Target, MovementTags, BlockingTags);
		outScores.Add(Target, Distance);
	}

	// Ensure all targets are in the map even if there are no paths to them
	for ( int i = 0; i < inTargets.Num(); i++)
	{
		ACGridTile* Target = inTargets[i];
		// If the target is not in the map, add it with a max possible distance
		if (!outScores.Contains(Target))
		{
			outScores.Add(Target, INT32_MAX);
			continue;
		}
		// Ensure no division by zero
		if(outScores[Target] == 0)
		{
			outScores[Target] = INT32_MAX;
		}
	}

	// Calculate the proximity score (Inverse of distance)
	for (auto& Pair : outScores)
	{
		Pair.Value = 1.0f / Pair.Value;
	}

	// Normalize the scores between 0 and 1
	float MaxScore = 0.0f;
	for (auto& Pair : outScores)
	{
		MaxScore = FMath::Max(MaxScore, Pair.Value);
	}

	for (auto& Pair : outScores)
	{
		Pair.Value /= MaxScore;
	}
	
}

TSet<ACGridTile*> UCGridUtilsLibrary::FloodFill(ACGridTile* inStart, int Depth,  const FGameplayTagContainer& MovementTags /*= FGameplayTagContainer()*/, const FGameplayTagContainer& MovementBlockingTags /*= FGameplayTagContainer()*/)
{
	//Default to regular straight movement.
	FGameplayTagContainer ValidMovements = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Movement);
	FGameplayTagContainer FilteredMoveTags = MovementTags.Filter(ValidMovements);
	if (FilteredMoveTags.IsEmpty())
		FilteredMoveTags.AddTag(TAG_Movement_Straight);

	TArray<ACGridTile*> OpenSet;
	OpenSet.Add(inStart);
	TArray<ACGridTile*> NextOpenSet;
	TSet<ACGridTile*> ClosedSet;
	ClosedSet.Add(inStart);
	for (int i = 0; i < Depth; i++)
	{
		for (ACGridTile* CurrentTile : OpenSet)
		{
			for (ACGridTile* Neighbour : ReachableInSingleStep(CurrentTile, FilteredMoveTags, MovementBlockingTags))
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

TSet<FVector2D> UCGridUtilsLibrary::FloodFillWithCoordinates(const FVector2D startCoord, int Depth, const FGameplayTagContainer& MovementTags)
{
	//Default to regular straight movement.
	FGameplayTagContainer ValidMovements = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Movement);
	FGameplayTagContainer FilteredMoveTags = MovementTags.Filter(ValidMovements);
	if (FilteredMoveTags.IsEmpty())
		FilteredMoveTags.AddTag(TAG_Movement_Straight);

	TArray<FVector2D> OpenSet;
	OpenSet.Add(startCoord);
	TArray<FVector2D> NextOpenSet;
	TSet<FVector2D> ClosedSet;
	ClosedSet.Add(startCoord);
	for (int i = 0; i < Depth; i++)
	{
		for (FVector2D CurrentTile : OpenSet)
		{
			for (FVector2D Neighbour : GetReachableCoordinates(CurrentTile, FilteredMoveTags))
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

TSet<FVector2D> UCGridUtilsLibrary::GetReachableCoordinates(FVector2D TileCoords, const FGameplayTagContainer& MovementTags /*= FGameplayTagContainer()*/) {
	TSet<FVector2D> NeighboursCoords;

	if (MovementTags.HasTag(TAG_Movement_Straight)) {
		NeighboursCoords.Append(ACGrid::GetTileNeighboursCoordinates(TileCoords));
	}
	if (MovementTags.HasTag(TAG_Movement_Diagonal)) {
		NeighboursCoords.Append(ACGrid::GetDiagonalTileNeighboursCoordinates(TileCoords));
	}
	if (MovementTags.HasTag(TAG_Movement_Knight)) {
		TSet<FVector2D> KnightMoves = {
			FVector2D(1, 2), FVector2D(2, 1), FVector2D(2, -1), FVector2D(1, -2),
			FVector2D(-1, -2), FVector2D(-2, -1), FVector2D(-2, 1), FVector2D(-1, 2)
		};
		for (FVector2D Move : KnightMoves) {
			NeighboursCoords.Add(TileCoords + Move);
		}
	}

	return NeighboursCoords;
}


TSet<ACGridTile*> UCGridUtilsLibrary::ReachableInSingleStep(ACGridTile* inTile, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& MovementBlockingTags /*= FGameplayTagContainer()*/) {
	ACGrid* Grid = inTile->GetParentGrid();

	TSet<FVector2D> Coordinates = GetReachableCoordinates(inTile->GetGridCoords(), MovementTags);
	
	TSet<ACGridTile*> Neighbours;

	for (FVector2D NeighbourCoords : Coordinates) {
		ACGridTile* NeighbourTile = Grid->GetTileFromCoords(NeighbourCoords);
		if (!NeighbourTile) continue;
		if (NeighbourTile->GetContent() && NeighbourTile->GetContent()->GetGameplayTags().HasAny(MovementBlockingTags)) continue;

		Neighbours.Add(NeighbourTile);
	}

	return Neighbours;
}

