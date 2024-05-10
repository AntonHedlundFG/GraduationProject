// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CActionWithTimer.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Utility/Logging/CLogManager.h"

void UCActionWithTimer::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	if (GetOwningComponent() && !AffectedUnit)
	{
		AffectedUnit = Cast<ACUnit>(GetOwningComponent()->GetOwner());
	}
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(ActionTags);
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

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
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

void UCActionWithTimer::OnTimerFinishes_Implementation(ACUnit* inAffectedUnit)
{
	LOG_WARNING("A timer has finished");
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(ActionTags);
}
