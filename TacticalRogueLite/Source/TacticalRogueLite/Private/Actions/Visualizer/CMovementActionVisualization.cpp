// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/Visualizer/CMovementActionVisualization.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Utility/Logging/CLogManager.h"

void UCMovementActionVisualization::Enter()
{
	TimePassed = 0.0f;
	MoveAction = Cast<UCMovementAction>(VisualizedAction);
	Path = MoveAction->Path;
}

bool UCMovementActionVisualization::Tick(float DeltaTime)
{

	if (MoveAction && MoveAction->MovedUnit)
	{
		if (MoveAction->Path.IsEmpty())
		{
			//Here we time out after a few seconds if no path is ever received
			CurrentDurationWithMissingPath += DeltaTime;
			if (CurrentDurationWithMissingPath >= TimeOutDurationForMissingPath)
			{
				LOG_WARNING("Timing out after %f seconds without path for visualized movement action", TimeOutDurationForMissingPath);
				if (IsValid(ParentSystem))
				{
					ACGameState* GameState = ParentSystem->GetWorld()->GetGameState<ACGameState>();
					for (ACUnit* Unit : GameState->GetCurrentTurnOrder(true))
					{
						Unit->SetActorLocation(Unit->GetTile()->GetActorLocation());
					}
				}
				return true;
			}
		}

		// Updates the current time frame of the animation
		TimePassed = FMath::Max(TimePassed + DeltaTime, 0.0f);

		// Update position based on the "alpha value", TimePassed / Duration.
		FVector Loc = LocationInPathAtTime(TimePassed);
		MoveAction->MovedUnit->SetActorLocation(Loc);

		//Different true conditions based on whether we're going forward or backward.
		//So we can call this implementation from RevertTick().
		return DeltaTime >= 0.0f
			? TimePassed >= (Path.Num() - 1) * DurationPerStep + (Path.Num() - 2) * DurationPerPause
			: TimePassed <= 0.0f;
	}

	return false;
}

bool UCMovementActionVisualization::RevertTick(float DeltaTime)
{
	return Tick(-DeltaTime * UndoSpeed);
}

FVector UCMovementActionVisualization::LocationInPathAtTime(float Time)
{
	Time = FMath::Clamp(Time, 0.0f, (Path.Num() - 1) * DurationPerStep + (Path.Num() - 2) * DurationPerPause);

	if (Time <= DurationPerStep)
		return FMath::Lerp(Path[0]->GetActorLocation(), Path[1]->GetActorLocation(), Time / DurationPerStep);
	Time -= DurationPerStep;

	int PathIndex = 1;
	while (PathIndex <= Path.Num() - 2)
	{
		if (Time <= DurationPerPause)
			return Path[PathIndex]->GetActorLocation();
		Time -= DurationPerPause;

		if (Time <= DurationPerStep)
			return FMath::Lerp(Path[PathIndex]->GetActorLocation(), Path[PathIndex + 1]->GetActorLocation(), Time / DurationPerStep);
		Time -= DurationPerStep;

		PathIndex++;
	}

	LOG_INFO("Something's wrong with LocationInPathAtTime()");
	return Path.Last()->GetActorLocation();
}
