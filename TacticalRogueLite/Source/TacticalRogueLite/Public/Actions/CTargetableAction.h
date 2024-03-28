// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CTargetableAction.generated.h"

class ACUnit;

UCLASS()
class TACTICALROGUELITE_API UCTargetableAction : public UCAction
{
	GENERATED_BODY()

public:

	ACGridTile* TargetTile;
	
	virtual TArray<ACGridTile*> GetReachableTiles(ACGridTile* inTile)
	{ return ReceiveGetReachableTiles(inTile); }

	virtual TArray<ACGridTile*> GetValidTargetTiles(ACGridTile* inTile)
	{ return ReceiveGetValidTargetTiles(inTile); }

	bool IsTileReachable(ACGridTile* fromTile, ACGridTile* toTile) 
	{ return GetValidTargetTiles(fromTile).Contains(toTile); }

#pragma endregion


#pragma region AngelScript overridables

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACGridTile* inUnit); 
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetReachableTiles(ACGridTile* inTile);

#pragma endregion
};
