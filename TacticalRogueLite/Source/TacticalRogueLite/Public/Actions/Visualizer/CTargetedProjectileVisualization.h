// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CTargetedProjectileVisualization.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCTargetedProjectileVisualization : public UCActionVisualization
{
	GENERATED_BODY()

public:

	virtual bool CanVisualizeAction_Implementation(UCAction* Action) override;

	virtual void Enter_Implementation() override;

	virtual bool Tick_Implementation(float DeltaTime) override;
	virtual bool RevertTick_Implementation(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class UCAction>> AffectedActionTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileType;

	//The total duration of the animation.
	UPROPERTY(EditAnywhere)
	float DurationPerTileInSeconds = 0.5f;

	//A speed multiplier which makes the undo animation faster
	float UndoSpeedMultiplier = 2.0f;

	UPROPERTY()
	TObjectPtr<AActor> SpawnedProjectile;

	//Calculated based on DurationPerTileInSeconds and attack distance
	float Duration;

	//Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	//A reference to the cast version of the action
	UPROPERTY()
	class UCTargetableAction* TargetableAction;

	//A reference to the actor being moved visually
	UPROPERTY()
	AActor* MovedActor;

	//Cached starting location
	FVector StartLocation;

	//Cached end location
	FVector TargetLocation;

	FVector MiddlePoint;

	//Height of middle point of curve in relation to distance of attack.
	UPROPERTY(EditAnywhere)
	float DistanceToHeightRatio = 0.3f;

	void CalculateSplinePoint(FVector& OutLocation, FRotator& OutRotation);
	void SpawnProjectile();
	void DespawnProjectile();
	
};
