// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemSlots.h"
#include "CItemAbility.generated.h"

class UCCommand;

/**
 * 
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCItemAbility : public UObject
{
	GENERATED_BODY()

public:

	virtual UCCommand* GenerateCommand() { return nullptr; }
	
};
