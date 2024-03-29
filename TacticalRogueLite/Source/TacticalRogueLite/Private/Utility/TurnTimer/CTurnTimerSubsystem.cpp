// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/TurnTimer/CTurnTimerSubsystem.h"

FTurnTimerHandle UCTurnTimerSubsystem::SetTimer(const int InNumberOfTurns, ACUnit* InAffectedUnit, const bool InbLoop, const EProgressTurnMethod InProgressTurnMethod, FOnTurnTimerExecute InDelegate)
{
	ActiveTurnTimers.Add(++LastTimerID, FTurnTimer());
	FTurnTimer& NewTimer = ActiveTurnTimers[LastTimerID];
	NewTimer.NumberOfTurns = InNumberOfTurns;
	NewTimer.TurnsRemaining = InNumberOfTurns;
	NewTimer.bLooping = InbLoop;
	NewTimer.ProgressTurnMethod = InProgressTurnMethod;
	NewTimer.AffectedUnit = InAffectedUnit;
	NewTimer.BoundDelegate = InDelegate;

	return FTurnTimerHandle(LastTimerID);
}

void UCTurnTimerSubsystem::ClearTimer(FTurnTimerHandle InHandle)
{
	if (IsTimerActive(InHandle))
		ActiveTurnTimers.Remove(InHandle.TimerID);
}

bool UCTurnTimerSubsystem::IsTimerActive(FTurnTimerHandle InHandle)
{
	return ActiveTurnTimers.Contains(InHandle.TimerID);
}

void UCTurnTimerSubsystem::NextTurn(const ACUnit* PreviousUnit, const ACUnit* NextUnit)
{
	for (auto TimerPair : ActiveTurnTimers)
	{
		FTurnTimer& Handle = TimerPair.Value;
		if (!(Handle.AffectedUnit == PreviousUnit && Handle.ProgressTurnMethod == EProgressTurnMethod::EPTM_EndOfTurn)
			&& !(Handle.AffectedUnit == NextUnit && Handle.ProgressTurnMethod == EProgressTurnMethod::EPTM_StartOfTurn)) continue;

		Handle.TurnsRemaining--;
		if (Handle.TurnsRemaining > 0) continue;

		Handle.BoundDelegate.ExecuteIfBound(Handle.AffectedUnit);
		if (Handle.bLooping)
		{
			//Reset timer, and keep running
			Handle.TurnsRemaining = Handle.NumberOfTurns;
		}
		else
		{
			//Remove timer and invalidate handle
			ActiveTurnTimers.Remove(TimerPair.Key);
		}
	}
}
