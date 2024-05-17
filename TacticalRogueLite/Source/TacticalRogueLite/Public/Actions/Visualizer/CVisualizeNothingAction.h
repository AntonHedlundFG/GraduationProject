// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CVisualizeNothingAction.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCVisualizeNothingAction : public UCActionVisualization
{
	GENERATED_BODY()

public:

	virtual void Enter() override {}
	virtual bool Tick(float DeltaTime) override { return true; }
	virtual bool RevertTick(float DeltaTime) override { return true; }

};
