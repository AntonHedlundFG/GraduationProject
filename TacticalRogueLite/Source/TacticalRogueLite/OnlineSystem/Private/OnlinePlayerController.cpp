// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalRogueLite\OnlineSystem\Public\OnlinePlayerController.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "TacticalRogueLite\OnlineSystem\Public\EpicOnlineSubsystem.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"

void AOnlinePlayerController::OnNetCleanup(UNetConnection* Connection)
{
	auto Subsystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsValid(Subsystem) && Subsystem->IsPlayerLoggedIn())
		{
			Subsystem->PlayerDisconnected(this);
		}
	}
	if (IsLocalController())
		Subsystem->DestroySession();

	Super::OnNetCleanup(Connection);
}

void AOnlinePlayerController::Server_ManualDisconnect_Implementation()
{
	auto Subsystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();
	Subsystem->UnregisterPlayer(this);
}

bool AOnlinePlayerController::Server_ManualDisconnect_Validate()
{
	return true;
}