// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCommand.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TACTICALROGUELITE_API UCCommand : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteCommand() {}
	virtual void UndoCommand() {}

};
