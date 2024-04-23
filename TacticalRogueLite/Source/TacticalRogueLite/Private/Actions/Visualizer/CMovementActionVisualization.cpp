// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CMovementActionVisualization.h"
#include "Actions/CMovementAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"

bool UCMovementActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return Action->IsA(UCMovementAction::StaticClass());
}

void UCMovementActionVisualization::Enter_Implementation()
{
	TimePassed = 0.0f;
	MoveAction = Cast<UCMovementAction>(VisualizedAction);
}

bool UCMovementActionVisualization::Tick_Implementation(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (MoveAction && MoveAction->MovedUnit)
	{
		// Fetch references and information
		if (!MovedActor)
		{
			StartLocation = MoveAction->FromTile->GetActorLocation() + FVector(0, 0, 100);
			TargetLocation = MoveAction->TargetTile->GetActorLocation() + FVector(0, 0, 100);
			MovedActor = MoveAction->MovedUnit;
		}

		// Update position based on the "alpha value", TimePassed / Duration.
		FVector Loc = FMath::Lerp(StartLocation, TargetLocation, TimePassed / Duration);
		MovedActor->SetActorLocation(Loc);

		//Different true conditions based on whether we're going forward or backward.
		//So we can call this implementation from RevertTick().
		return DeltaTime >= 0.0f ? TimePassed >= Duration : TimePassed <= 0.0f;
	}

	return false;
}

bool UCMovementActionVisualization::RevertTick_Implementation(float DeltaTime)
{
	return Tick_Implementation(-DeltaTime * UndoSpeed);
}
