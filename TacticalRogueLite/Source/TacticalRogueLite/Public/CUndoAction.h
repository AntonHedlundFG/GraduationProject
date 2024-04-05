// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CUndoAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCUndoAction : public UCAction
{
	GENERATED_BODY()

protected:

	UCUndoAction() { bIsUndone = true; }
	
public:

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<TSoftObjectPtr<UCAction>> UndoneActions;

};
