// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CTargetableAction.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCTargetableAction : public UCAction
{
	GENERATED_BODY()
	
	virtual TArray<ACGridTile*> GetReachableTiles(ACGridTile* inTile)
	{ return ReceiveGetReachableTiles(inTile); }


protected:

	virtual TArray<ACGridTile*> GetValidTargetTilesInternal(ACUnit* inUnit)
	{ return ReceiveGetValidTargetTiles(inUnit); }

#pragma endregion


#pragma region AngelScript overridables

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACUnit* inUnit); 
	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetReachableTiles(ACGridTile* inTile);

#pragma endregion
};
