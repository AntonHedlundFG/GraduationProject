// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CDeathActionVisualization.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCDeathActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()
	
	virtual void Enter() override;

	virtual bool Tick(float DeltaTime) override;
	virtual bool RevertTick(float DeltaTime) override;

	//Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	//The total duration of the animation.
	float Duration = 1.0f;

	//A reference to the cast version of the action
	UPROPERTY()
	class UCDeathAction* DeathAction;

	//A reference to the actor being moved visually
	UPROPERTY()
	AActor* DeadActor;

	//A speed multiplier which makes the undo animation faster
	float UndoSpeed = 2.0f;

	FVector StartingScale = FVector(1.0f, 1.0f, 1.0f);
	FVector TargetScale = FVector(0.1f, 0.1f, 0.1f);
	FVector StartingLocation;
	FVector DeadLocation = FVector(10000, 10000, 10000);

};
