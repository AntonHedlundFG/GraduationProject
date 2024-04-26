// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CActionWithTimer.h"

#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"

void UCActionWithTimer::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	if (!AffectedUnit)
		AffectedUnit = Cast<ACUnit>(Instigator);


	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

	
}

void UCActionWithTimer::UndoAction(AActor* Instigator)
{
	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
		Subsystem->ClearTimer(TimerHandle);

	LOG_INFO("Clearing a timer");

	Super::UndoAction(Instigator);

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

void UCActionWithTimer::BindTimer()
{
	TimerDelegate.BindUFunction(this, FName("OnTimerFinishes"));
	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
		TimerHandle = Subsystem->SetTimer(NumberOfTurns, AffectedUnit, bShouldLoop, ProgressTurnMethod, TimerDelegate);
	LOG_INFO("Setting a timer");
}

void UCActionWithTimer::OnTimerFinishes_Implementation(ACUnit* inAffectedUnit)
{
	LOG_WARNING("An unassigned timer has finished");
}
