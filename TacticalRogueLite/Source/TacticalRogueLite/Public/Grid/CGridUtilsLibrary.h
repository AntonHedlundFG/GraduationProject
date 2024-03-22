// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGridUtilsLibrary.generated.h"


class UCItem;
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
	static TArray<ACGridTile*> BFS_Pathfinding(UCItem* inItem, ACGridTile* inStart, const ACGridTile* inTarget);
	UFUNCTION(ScriptCallable)
	static TSet<ACGridTile*> FloodFill(UCItem* inItem, ACGridTile* inStart, int Depth);
	
};
