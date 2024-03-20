// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "CGameMode.h"

void ACPlayerController::Server_UseObject_Implementation(ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex)
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling ability use."))
		return;
	}
	if (!GameMode->TryAbilityUse(this, inUnit, inSlot, inTileIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability use failed in GameMode"));
		return;
	}
}

void ACPlayerController::Server_TryUndo_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling undo."))
			return;
	}
	if (!GameMode->TryUndo(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Undoing failed in GameMode"));
		return;
	}
}

void ACPlayerController::Server_TryEndTurn_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling end turn."))
			return;
	}
	if (!GameMode->TryEndTurn(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Endturn failed in GameMode"));
		return;
	}
}