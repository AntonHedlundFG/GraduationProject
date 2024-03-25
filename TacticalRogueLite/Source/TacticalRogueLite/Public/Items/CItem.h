// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItem.generated.h"

class UCCommand;
class ACUnit;
class ACGridTile;

/** If making a C++ extension that is also extended by AngelScript/Blueprints, 
* make sure you call the Super::GenerateAbilityCommand() function manually. 
* If the implementation is entirely C++, do not make a Super:: call.
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCItem : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UTexture2D> AbilityIcon;

	virtual UCCommand* GenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile)
	{
		return ReceiveGenerateAbilityCommand(inController, inUnit, inTargetTile);
	}

	UFUNCTION(BlueprintImplementableEvent)
	UCCommand* ReceiveGenerateAbilityCommand(AController* inController, ACUnit* inUnit, ACGridTile* inTargetTile);

	virtual TArray<ACGridTile*> GetReachableTiles(ACGridTile* inTile) 
	{ 
		return ReceiveGetReachableTiles(inTile); 
	}

	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetReachableTiles(ACGridTile* inTile);
	
	virtual TArray<ACGridTile*> GetValidTargetTiles(ACUnit* inUnit) 
	{ 
		return ReceiveGetValidTargetTiles(inUnit); 
	}

	UFUNCTION(BlueprintImplementableEvent)
	TArray<ACGridTile*> ReceiveGetValidTargetTiles(ACUnit* inUnit);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	bool IsValidTargetTile(ACUnit* inUnit, ACGridTile* inTargetTile);

	/* Removed, we should just be using ACGridTile* instead of int indexes
	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	bool IsValidTargetTileIndex(ACUnit* inUnit, const int inTargetTileIndex);
	*/

};