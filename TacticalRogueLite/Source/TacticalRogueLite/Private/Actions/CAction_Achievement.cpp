// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_Achievement.h"

#include "CGameInstance.h"
#include "Achievements/CAchievementSubsystem.h"

class UCGameInstance;

void UCAction_Achievement::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	
}

void UCAction_Achievement::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCAchievementSubsystem* Achievements = GI->GetSubsystem<UCAchievementSubsystem>();
	if (Achievements)
		{
			//Pass along 'OwnedTags' for the system to understand which achievements to unlock.
			Achievements->UnlockAchievement(ActionTags);
		}
}

void UCAction_Achievement::UndoAction_Implementation(AActor* Instigator)
{
	Super::UndoAction_Implementation(Instigator);
}
