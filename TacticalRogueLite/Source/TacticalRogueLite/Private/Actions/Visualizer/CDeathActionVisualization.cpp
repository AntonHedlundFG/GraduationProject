// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CDeathActionVisualization.h"
#include "Actions/CDeathAction.h"
#include "GridContent/CUnit.h"

bool UCDeathActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return Action->IsA(UCDeathAction::StaticClass());
}

void UCDeathActionVisualization::Enter_Implementation()
{
	TimePassed = 0.0f;
	DeathAction = Cast<UCDeathAction>(VisualizedAction);
}

bool UCDeathActionVisualization::Tick_Implementation(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (DeathAction && DeathAction->AffectedUnit)
	{
		// Fetch references and information
		if (!DeadActor)
		{
			DeadActor = DeathAction->AffectedUnit;
			StartingLocation = DeadActor->GetActorLocation();
		}

		// Update position based on the "alpha value", TimePassed / Duration.
		FVector NewScale = FMath::Lerp(StartingScale, TargetScale, TimePassed / Duration);
		DeadActor->SetActorScale3D(NewScale);

		//Different true conditions based on whether we're going forward or backward.
		//So we can call this implementation from RevertTick().
		if (DeltaTime > 0.0f)
		{
			if (TimePassed >= Duration)
			{
				DeadActor->SetActorLocation(DeadLocation);
				return true;
			}
		}
		else
		{
			DeadActor->SetActorLocation(StartingLocation);
			if (TimePassed <= 0.0f)
			{
				return true;
			}
		}
		return false;
	}

	return false;
}

bool UCDeathActionVisualization::RevertTick_Implementation(float DeltaTime)
{
	return Tick_Implementation(-DeltaTime * UndoSpeed);
}
