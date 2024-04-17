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
	UFUNCTION(ScriptCallable)
	static TArray<FVector2D> StraightDirections();
	UFUNCTION(ScriptCallable)
	static TArray<FVector2D> DiagonalDirections();
	UFUNCTION(ScriptCallable)
	static TArray<ACGridTile*> BFS_Pathfinding(ACGridTile* inStart, const ACGridTile* inTarget, const FGameplayTagContainer& MovementTags, const FGameplayTagContainer& BlockingTags, bool bIncludeTargetInPath = true);
	UFUNCTION(ScriptCallable)
	static TArray<ACGridTile*> AStar_Pathfinding(ACGridTile* inStart, ACGridTile* inTarget, const FGameplayTagContainer& MovementTags,	const FGameplayTagContainer& BlockingTags, bool bIncludeTargetInPath = true);
	static float GetManhattanDistance(const ACGridTile* From, const ACGridTile* To);
	UFUNCTION(ScriptCallable)
	static ACGridContent* GetClosestGridContent(ACGridTile* inStart, TArray<ACGridContent*>& ContentArray, const FGameplayTagContainer& MovementTags, const
	                                            FGameplayTagContainer& BlockingTags);

	//For example, MovementMethods can contain the tag "Movement_Straight" and "Movement_Diagonal".
	//Or only "Movement_Diagonal"
	UFUNCTION(ScriptCallable)
	static TSet<ACGridTile*> FloodFill(ACGridTile* inStart, int Depth, FGameplayTagContainer MovementMethods = FGameplayTagContainer(), bool BlockedByUnits = true);
	
	UFUNCTION(ScriptCallable)
	static TSet<ACGridTile*> ReachableInSingleStep(FGameplayTagContainer MovementMethods, ACGridTile* inTile);

};
