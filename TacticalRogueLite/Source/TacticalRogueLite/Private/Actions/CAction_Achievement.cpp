// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_Achievement.h"

#include "CGameInstance.h"
#include "Achievements/CAchievementSubsystem.h"

class UCGameInstance;

void UCAction_Achievement::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);
	
}

void UCAction_Achievement::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);

	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCAchievementSubsystem* Achievements = GI->GetSubsystem<UCAchievementSubsystem>();
	if (Achievements)
		{
			//Pass along 'OwnedTags' for the system to understand which achievements to unlock.
			Achievements->UnlockAchievement(ActionTags);
		}
}

void UCAction_Achievement::UndoAction(AActor* Instigator)
{
	Super::UndoAction(Instigator);
}
