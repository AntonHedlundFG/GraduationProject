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

	virtual void Enter() override;

	virtual bool Tick(float DeltaTime) override;
	virtual bool RevertTick(float DeltaTime) override;

protected:

	/// <summary>
	/// This is the class of actor that should represent the projectile.
	/// It is entirely local and its location&rotation will be managed by this class.
	/// It is despawned by this class at the end of its movement.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileType;

	/// <summary>
	/// This actor will be spawned when the animation finishes its duration
	/// It's lifetime is set by OnHitEffectLifetime
	/// Useful for spawning VFX on impact.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> OnHitEffectType;

	/// <summary>
	/// This actor will be spawned before the projectile is spawned (if there is one.)
	/// It's lifetime is set by BeforeLaunchProjectileEffectLifetime
	/// Useful for spawning VFX on the ability's user.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BeforeLaunchProjectileEffectType;

	//Determines lifetime of the actor of type OnHitEffectType.
	//If 0 or less, no lifetime is set and you have to manage its lifetime elsewhere.
	UPROPERTY(EditAnywhere)
	float OnHitEffectLifetime = 0.0f;
	UPROPERTY(EditAnywhere)
	FVector TargetLocationOffset;

	//Determines lifetime of the actor of type BeforeLaunchProjectileEffectType.
	//If 0 or less, no lifetime is set and you have to manage its lifetime elsewhere.
	UPROPERTY(EditAnywhere)
	float BeforeLaunchProjectileEffectLifetime = 0.0f;

	//If you want the BeforeLaunchProjectileEffectType actor to play for a duration
	// before lauching the projectile, use this property.
	UPROPERTY(EditAnywhere)
	float DelayBeforeLaunchingProjectile = 0.0f;
	float TimePassedBeforeProjectile = 0.0f;

	//For each tile distance (100.0f) the projectile travels, its total duration
	//increases by this much.
	UPROPERTY(EditAnywhere)
	float DurationPerTileInSeconds = 0.5f;

	//Height of middle point of curve in relation to distance of attack.
	UPROPERTY(EditAnywhere)
	float DistanceToHeightRatio = 0.3f;

	//A speed multiplier which makes the undo animation faster
	UPROPERTY(EditAnywhere)
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

	FVector StartLocation;
	FVector TargetLocation;
	//Cached middle location used for spline calculations
	FVector MiddlePoint;	

	void CalculateSplinePoint(FVector& OutLocation, FRotator& OutRotation);
	void SpawnProjectile();
	void DespawnProjectile();
	void SpawnOnHitEffect();
	void SpawnLaunchEffect();
	
};
