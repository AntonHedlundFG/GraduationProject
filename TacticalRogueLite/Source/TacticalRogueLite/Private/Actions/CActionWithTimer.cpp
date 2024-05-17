// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CActionWithTimer.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Utility/Logging/CLogManager.h"

void UCActionWithTimer::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	ensure(GetOwningComponent());
	
	AffectedUnit = Cast<ACUnit>(GetOwningComponent()->GetOwner());
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(ActionTags);
	if (GetOwningComponent()->OnActionStarted.IsBound())
		GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

	BindTimer();	
}

void UCActionWithTimer::UndoAction(AActor* Instigator)
{
	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
		Subsystem->ClearTimer(TimerHandle);

	LOG_INFO("Clearing a timer");

	Super::UndoAction(Instigator);


	if (GetOwningComponent()->OnActionUndo.IsBound())
		GetOwningComponent()->OnActionUndo.Broadcast(GetOwningComponent(), this); 
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(ActionTags);
}


void UCActionWithTimer::BindTimer()
{
	TimerDelegate.BindUFunction(this, FName("OnTimerFinishes"));
	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
		TimerHandle = Subsystem->SetTimer(NumberOfTurns, AffectedUnit, bShouldLoop, ProgressTurnMethod, TimerDelegate);
	LOG_INFO("Setting a timer");
}

void UCActionWithTimer::OnRep_RepData(const FActionRepData& OldData)
{
	if (RepData.bIsRunning && !OldData.bIsRunning)
	{
		if (GetOwningComponent()->OnActionStarted.IsBound())
			GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
	}
	if (OldData.bIsRunning && !RepData.bIsRunning) //Has been active but after last rep call isn't.
	{
		if (GetOwningComponent()->OnActionStopped.IsBound())
			GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
	}
}

void UCActionWithTimer::OnRep_bIsUndone()
{
	if (GetOwningComponent()->OnActionUndo.IsBound())
		GetOwningComponent()->OnActionUndo.Broadcast(GetOwningComponent(), this);
}

void UCActionWithTimer::OnTimerFinishes(ACUnit* inAffectedUnit)
{
	LOG_WARNING("A timer has finished");
	if (GetOwningComponent()->OnActionStopped.IsBound())
		GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(ActionTags);

	RepData.bIsRunning = false; //To replicate OnActionStopped to clients.
	ReceiveOnTimerFinishes(inAffectedUnit);
}

void UCActionWithTimer::ReceiveOnTimerFinishes_Implementation(ACUnit* inAffectedUnit)
{
	
}


