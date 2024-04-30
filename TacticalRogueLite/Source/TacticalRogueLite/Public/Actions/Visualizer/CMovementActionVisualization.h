// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CMovementActionVisualization.generated.h"

UCLASS()
class TACTICALROGUELITE_API UCMovementActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()

	virtual bool CanVisualizeAction_Implementation(UCAction* Action) override;
	
	virtual void Enter_Implementation() override;

	virtual bool Tick_Implementation(float DeltaTime) override;
	virtual bool RevertTick_Implementation(float DeltaTime) override;

	//Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	//The total duration of the movement.
	float Duration = 1.0f;

	//A reference to the cast version of the action
	UPROPERTY()
	class UCMovementAction* MoveAction;

	//A reference to the actor being moved visually
	UPROPERTY()
	AActor* MovedActor;

	//A speed multiplier which makes the undo animation faster
	float UndoSpeed = 5.0f;

	//Cached starting location
	FVector StartLocation;
	
	//Cached end location
	FVector TargetLocation;

};
