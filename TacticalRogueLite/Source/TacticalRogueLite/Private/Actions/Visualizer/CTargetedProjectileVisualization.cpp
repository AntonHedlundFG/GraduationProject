// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CTargetedProjectileVisualization.h"
#include "Actions\CAction.h"
#include "Actions\CTargetableAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Grid\CGridTile.h"
#include "Actions\CActionComponent.h"

bool UCTargetedProjectileVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	for (TSubclassOf<UCAction> ActionType : AffectedActionTypes)
	{
		if (Action->IsA(ActionType))
			return true;
	}
	return false;
}

void UCTargetedProjectileVisualization::Enter_Implementation()
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
	}

}

bool UCTargetedProjectileVisualization::Tick_Implementation(float DeltaTime)
{
	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (TargetableAction)
	{
		FVector Location;
		FRotator Rotation;
		CalculateSplinePoint(Location, Rotation);

		if (!SpawnedProjectile)
		{
			SpawnProjectile();
			if (!SpawnedProjectile)
				return true; //Something went wrong when spawning, move on.
		}

		SpawnedProjectile->SetActorLocationAndRotation(Location, Rotation);

		const bool bIsFinished = DeltaTime >= 0.0f ? TimePassed >= Duration : TimePassed <= 0.0f;

		if (bIsFinished)
		{
			LOG_INFO("We're here");
			DespawnProjectile();
		}

		return bIsFinished;
	}

	return false;
}

bool UCTargetedProjectileVisualization::RevertTick_Implementation(float DeltaTime)
{
	return Tick_Implementation(-DeltaTime * UndoSpeedMultiplier);
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
	GetWorld()->DestroyActor(SpawnedProjectile);
	SpawnedProjectile = nullptr;
}
