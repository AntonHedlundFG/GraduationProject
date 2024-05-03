// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CResurrectVisualization.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCResurrectVisualization : public UCActionVisualization
{
	GENERATED_BODY()
	virtual bool CanVisualizeAction(UCAction* Action) override;

	virtual void Enter() override;

	virtual bool Tick(float DeltaTime) override;
	virtual bool RevertTick(float DeltaTime) override;

	//Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	//The total duration of the animation.
	float Duration = 0.2f;

	//A reference to the cast version of the action
	UPROPERTY()
	class UCResurrectAction* ResurrectAction;

	//A reference to the actor being moved visually
	UPROPERTY()
	AActor* AffectedActor;

	//A speed multiplier which makes the undo animation faster
	float UndoSpeed = 2.0f;

	FVector StartingScale = FVector(1.0f, 1.0f, 1.0f);
	FVector TargetScale = FVector(1.0f, 0.1f, 1.0f);
	FVector DeadLocation = FVector(10000, 10000, 10000);
};
