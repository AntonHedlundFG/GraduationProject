// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "CGridUtilsLibrary.generated.h"

class ACGridContent;
class UCTargetableAction;
class ACGridTile;

/**
 * 
 */
UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCGridUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<FVector2D> StraightDirections();
	UFUNCTION(BlueprintCallable)
	static TArray<FVector2D> DiagonalDirections();
	UFUNCTION(BlueprintCallable)
	static TArray<ACGridTile*> BFS_Pathfinding(ACGridTile* inStart, const ACGridTile* inTarget, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer
	                                           & BlockingTags = FGameplayTagContainer());
	UFUNCTION(BlueprintCallable)
	static TArray<ACGridTile*> AStar_Pathfinding(ACGridTile* inStart, ACGridTile* inTarget, const FGameplayTagContainer& MovementTags,	const FGameplayTagContainer& BlockingTags = FGameplayTagContainer());
	UFUNCTION(BlueprintCallable)
	static float GetManhattanDistance(const ACGridTile* From, const ACGridTile* To);
	UFUNCTION(BlueprintCallable)
	static ACGridContent* GetClosestGridContent(ACGridTile* inStart, TArray<ACGridContent*>& ContentArray, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags);
	UFUNCTION(BlueprintCallable)
	static int32 GetAStarPathFindingDistance(ACGridTile* inStart, ACGridTile* inTarget, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer&
	                                         BlockingTags);
	UFUNCTION(BlueprintCallable)
	static int32 GetBFSPathFindingDistance(ACGridTile* inStart, ACGridTile* inTarget, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags);
	UFUNCTION(BlueprintCallable)
	static void GetProximityScore(ACGridTile* inStart, const TArray<ACGridTile*>& inTargets, const FGameplayTagContainer& MovementTags, const
	                              FGameplayTagContainer& BlockingTags, TMap<ACGridTile*, float>& outScores);

	//For example, MovementMethods can contain the tag "Movement_Straight" and "Movement_Diagonal".
	//Or only "Movement_Diagonal"
	UFUNCTION(BlueprintCallable)
	static TSet<ACGridTile*> FloodFill(ACGridTile* inStart, int Depth, const FGameplayTagContainer& MovementTags = FGameplayTagContainer(), const FGameplayTagContainer& MovementBlockingTags = FGameplayTagContainer());

	UFUNCTION(BlueprintCallable)
	static TSet<FVector2D> FloodFillWithCoordinates(FVector2D startCoord, int Depth, const FGameplayTagContainer& MovementTags = FGameplayTagContainer());
	
	// Returns a set of coordinates that are reachable from the input tile.
	// No filters are applied, so this will return all reachable coordinates even if the tiles are blocked or if there are no tiles at the coordinates.
	UFUNCTION(BlueprintCallable)
	static TSet<FVector2D> GetReachableCoordinates(FVector2D TileCoords, const FGameplayTagContainer& MovementTags = FGameplayTagContainer());

	// Returns a set of tiles that are reachable from the input tile. 
	UFUNCTION(BlueprintCallable)
	static TSet<ACGridTile*> ReachableInSingleStep(ACGridTile* inTile, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& MovementBlockingTags = FGameplayTagContainer());

};
