// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "Actions/CMovementAction.h"
#include "CMovementActionVisualization.generated.h"

UCLASS()
class TACTICALROGUELITE_API UCMovementActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()

public:

	virtual bool CanVisualizeAction(UCAction* Action) override;
	
	virtual void Enter() override;

	virtual bool Tick(float DeltaTime) override;
	virtual bool RevertTick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCMovementAction> VisualizedClass;

	//Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	//The duration of each step
	UPROPERTY(EditDefaultsOnly, Category = "Visualization", meta = (ClampMin = 0.5f, ClampMax = 5.0f))
	float DurationPerStep = 1.0f;
	//The pause duration in between each step
	UPROPERTY(EditDefaultsOnly, Category = "Visualization", meta = (ClampMin = 0.5f, ClampMax = 5.0f))
	float DurationPerPause = 0.2f;

	//A reference to the cast version of the action
	UPROPERTY()
	class UCMovementAction* MoveAction;

	UPROPERTY()
	TArray<class ACGridTile*> Path;

	//A speed multiplier which makes the undo animation faster
	UPROPERTY(EditDefaultsOnly, Category = "Visualization", meta = (ClampMin = 1.0f, ClampMax = 10.0f))
	float UndoSpeed = 5.0f;

	//Cached starting location
	FVector StartLocation;
	
	//Cached end location
	FVector TargetLocation;

	FVector LocationInPathAtTime(float Time);

};
