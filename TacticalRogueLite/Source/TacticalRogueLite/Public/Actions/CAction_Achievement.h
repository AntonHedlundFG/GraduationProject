// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Achievement.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCAction_Achievement : public UCAction
{
	GENERATED_BODY()

public:

	virtual void StartAction(AActor* Instigator) override;
	virtual void StopAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;
};
