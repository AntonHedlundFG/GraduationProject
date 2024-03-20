// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItem.generated.h"

class UCCommand;
class ACUnit;

/** If making a C++ extension that is also extended by AngelScript/Blueprints, 
* make sure you call the Super::GenerateAbilityCommand() function manually. 
* If the implementation is entirely C++, do not make a Super:: call.
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCItem : public UDataAsset
{
	GENERATED_BODY()

public:

	virtual UCCommand* GenerateAbilityCommand(AController* inController, ACUnit* inUnit, const int inTileIndex)
	{
		return ReceiveGenerateAbilityCommand(inController, inUnit, inTileIndex);
	}

	UFUNCTION(BlueprintImplementableEvent)
	UCCommand* ReceiveGenerateAbilityCommand(AController* inController, ACUnit* inUnit, const int inTileIndex);
	
};