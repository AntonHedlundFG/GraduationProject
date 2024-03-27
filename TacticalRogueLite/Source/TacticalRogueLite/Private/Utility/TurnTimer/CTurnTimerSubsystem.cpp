// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/TurnTimer/CTurnTimerSubsystem.h"

void UCTurnTimerSubsystem::SetTimer(FTurnTimerHandle& InOutHandle, const int InNumberOfTurns, ACUnit* InAffectedUnit, const bool InbLoop, const EProgressTurnMethod InProgressTurnMethod)
{
	InOutHandle.bLooping = InbLoop;
	InOutHandle.NumberOfTurns = InNumberOfTurns;
	InOutHandle.TurnsRemaining = InNumberOfTurns;
	InOutHandle.TimerID = ++LastTimerID;
	InOutHandle.ProgressTurnMethod = InProgressTurnMethod;
	InOutHandle.AffectedUnit = InAffectedUnit;

	ActiveTurnTimers.Add(InOutHandle.TimerID, &InOutHandle);
	InOutHandle.bIsValid = true;
}

void UCTurnTimerSubsystem::ClearTimer(FTurnTimerHandle& InOutHandle)
{
	ActiveTurnTimers.Remove(InOutHandle.TimerID);
	InOutHandle.bIsValid = false;
}

void UCTurnTimerSubsystem::NextTurn(const ACUnit* PreviousUnit, const ACUnit* NextUnit)
{
	for (auto TimerPair : ActiveTurnTimers)
	{
		FTurnTimerHandle* Handle = TimerPair.Value;
		if (!(Handle->AffectedUnit == PreviousUnit && Handle->ProgressTurnMethod == EPTM_EndOfTurn)
			&& !(Handle->AffectedUnit == NextUnit && Handle->ProgressTurnMethod == EPTM_StartOfTurn)) continue;

		Handle->TurnsRemaining--;
		if (Handle->TurnsRemaining > 0) continue;

		Handle->OnTurnTimerExecute.Broadcast(Handle->AffectedUnit);
		if (Handle->bLooping)
		{
			//Reset timer, and keep running
			Handle->TurnsRemaining = Handle->NumberOfTurns;
		}
		else
		{
			//Remove timer and invalidate handle
			ActiveTurnTimers.Remove(TimerPair.Key);
			Handle->bIsValid = false;
		}
	}
}
