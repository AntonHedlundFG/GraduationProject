// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "CGameMode.h"

void ACPlayerController::Server_UseObject_Implementation(ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex)
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling ability use."))
		return;
	}
	if (!GameMode->TryAbilityUse(this, inUnit, inSlot, inTileIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability use failed due to bad input"));
		return;
	}
}
