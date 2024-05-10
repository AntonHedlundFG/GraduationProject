// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CTargetedProjectileVisualization.h"
#include "Actions\CAction.h"
#include "Actions\CTargetableAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Grid\CGridTile.h"
#include "Actions\CActionComponent.h"

void UCTargetedProjectileVisualization::Enter()
{
	TimePassed = 0.0f;
	TargetableAction = Cast<UCTargetableAction>(VisualizedAction);
	if (TargetableAction)
	{
		TargetLocation = TargetableAction->TargetTile->GetActorLocation() + FVector(0, 0, 100);
		StartLocation = TargetableAction->GetActionComp()->GetOwner()->GetActorLocation();
		const float AttackDistance = FVector::Dist(TargetLocation, StartLocation);
		MiddlePoint = (TargetLocation + StartLocation) / 2.0f + FVector(0, 0, AttackDistance * DistanceToHeightRatio);
		Duration = DurationPerTileInSeconds * AttackDistance / 100.0f;

		SpawnLaunchEffect();
	}

}

bool UCTargetedProjectileVisualization::Tick(float DeltaTime)
{
	if (TimePassedBeforeProjectile < DelayBeforeLaunchingProjectile)
	{
		TimePassedBeforeProjectile += DeltaTime;
		DeltaTime = TimePassedBeforeProjectile - DelayBeforeLaunchingProjectile;
		
		if (DeltaTime <= 0.0f) 
			return false;
	}

	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (TargetableAction)
	{
		FVector Location;
		FRotator Rotation;
		CalculateSplinePoint(Location, Rotation);

		bool bIsFinished = false;

		if (!SpawnedProjectile)
			SpawnProjectile();

		if (!SpawnedProjectile)
			bIsFinished = true; //No projectile was spawned, move on.
		else
			SpawnedProjectile->SetActorLocationAndRotation(Location, Rotation);

		//Whether we've finished depends on whether we're ticking forward or backward (do or undo)
		bIsFinished |= DeltaTime >= 0.0f ? TimePassed >= Duration : TimePassed <= 0.0f;

		if (bIsFinished)
		{
			DespawnProjectile(); //We've finished ticking
			if (DeltaTime > 0.0f)
				SpawnOnHitEffect(); //We've finished ticking -forward-
		}

		return bIsFinished;
	}

	return false;
}

bool UCTargetedProjectileVisualization::RevertTick(float DeltaTime)
{
	return Tick(-DeltaTime * UndoSpeedMultiplier);
}

void UCTargetedProjectileVisualization::CalculateSplinePoint(FVector& OutLocation, FRotator& OutRotation)
{
	const float Alpha = FMath::Clamp(TimePassed / Duration, 0.0f, 1.0f);

	const FVector PointA = StartLocation * (1 - Alpha) + MiddlePoint * Alpha;
	const FVector PointB = MiddlePoint * (1 - Alpha) + TargetLocation * Alpha;
	const FVector Forward = (PointB - PointA).GetSafeNormal();
	const FVector Right = Forward.Cross(FVector::UpVector);
	const FVector Up = Right.Cross(Forward);

	OutLocation = PointA * (1 - Alpha) + PointB * Alpha;
	OutRotation = UKismetMathLibrary::MakeRotationFromAxes(Forward, Right, Up);
}

void UCTargetedProjectileVisualization::SpawnProjectile()
{
	if (IsValid(ProjectileType))
		SpawnedProjectile = GetWorld()->SpawnActor(ProjectileType);
}

void UCTargetedProjectileVisualization::DespawnProjectile()
{
	if (SpawnedProjectile)
		GetWorld()->DestroyActor(SpawnedProjectile);
	SpawnedProjectile = nullptr;
}

void UCTargetedProjectileVisualization::SpawnOnHitEffect()
{
	if (IsValid(OnHitEffectType))
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(OnHitEffectType);
		SpawnedActor->SetActorLocation(TargetLocation);
		if (OnHitEffectLifetime > 0.0f)
			SpawnedActor->SetLifeSpan(OnHitEffectLifetime);
	}
}

void UCTargetedProjectileVisualization::SpawnLaunchEffect()
{
	if (IsValid(BeforeLaunchProjectileEffectType))
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(BeforeLaunchProjectileEffectType);
		SpawnedActor->SetActorLocation(StartLocation);
		if (OnHitEffectLifetime > 0.0f)
			SpawnedActor->SetLifeSpan(BeforeLaunchProjectileEffectLifetime);
	}
}
