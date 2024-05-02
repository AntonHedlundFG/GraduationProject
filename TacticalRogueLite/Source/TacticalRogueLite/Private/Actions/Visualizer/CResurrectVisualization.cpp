// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CResurrectVisualization.h"
#include "Actions/CResurrectAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"

bool UCResurrectVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return Action->IsA(UCResurrectAction::StaticClass());
}

void UCResurrectVisualization::Enter_Implementation()
{
	TimePassed = 0.0f;
	ResurrectAction = Cast<UCResurrectAction>(VisualizedAction);
}

bool UCResurrectVisualization::Tick_Implementation(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (ResurrectAction && ResurrectAction->AffectedUnit)
	{

		float AlphaValue = 2.0f * TimePassed / Duration;
		if (AlphaValue > 1.0f)
			AlphaValue = (2.0f - AlphaValue);
		// Update position based on the "alpha value", TimePassed / Duration.
		FVector NewScale = FMath::Lerp(StartingScale, TargetScale, AlphaValue);
		ResurrectAction->AffectedUnit->SetActorScale3D(NewScale);

		if (TimePassed < Duration / 2.0f)
		{
			if (ResurrectAction->PreviousTile)
				ResurrectAction->AffectedUnit->SetActorLocation(ResurrectAction->PreviousTile->GetActorLocation() + FVector(0, 0, 100));
		}
		else
		{
			ResurrectAction->AffectedUnit->SetActorLocation(ResurrectAction->ResurrectOnTile->GetActorLocation() + FVector(0, 0, 100));
		}

		return DeltaTime >= 0.0f ? TimePassed >= Duration : TimePassed <= 0.0f;
	}

	return false;
}

bool UCResurrectVisualization::RevertTick_Implementation(float DeltaTime)
{
	return Tick_Implementation(-DeltaTime * UndoSpeed);
}
