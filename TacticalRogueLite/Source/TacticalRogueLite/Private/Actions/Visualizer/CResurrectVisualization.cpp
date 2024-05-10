// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Visualizer/CResurrectVisualization.h"
#include "Actions/CResurrectAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"

void UCResurrectVisualization::Enter()
{
	TimePassed = 0.0f;
  	ResurrectAction = Cast<UCResurrectAction>(VisualizedAction);
}

bool UCResurrectVisualization::Tick(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = FMath::Clamp(TimePassed + DeltaTime, 0.0f, Duration);

	if (ResurrectAction)
	{
		if(ResurrectAction->AffectedUnit)
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
						ResurrectAction->AffectedUnit->SetActorLocation(ResurrectAction->PreviousTile->GetActorLocation());
				}
				else
				{
					ResurrectAction->AffectedUnit->SetActorLocation(ResurrectAction->ResurrectOnTile->GetActorLocation());
				}

				return DeltaTime >= 0.0f ? TimePassed >= Duration : TimePassed <= 0.0f;
		}
	}
	else
	{
		return  true;
	}

	return false;
}

bool UCResurrectVisualization::RevertTick(float DeltaTime)
{
	return Tick(-DeltaTime * UndoSpeed);
}
