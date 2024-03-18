// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "TacticalRogueLite\OnlineSystem\Public\EpicOnlineSubsystem.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlinePlayerState.h"
#include "GameFramework/PlayerState.h"

void AOnlineGameMode::PostLogin(APlayerController* NewPlayer)
{
	auto Subsystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();

	//Bypass online functionality, usually in PIE mode
	if (!Subsystem || !Subsystem->IsPlayerLoggedIn())
	{
		Super::PostLogin(NewPlayer);

		if (auto* CastState = Cast<AOnlinePlayerState>(NewPlayer->PlayerState))
		{
			if (!NewPlayer->IsLocalPlayerController() && CastState->PlayerIndex <= 0)
			{
				CastState->PlayerIndex = NextPlayerIndex;
				NextPlayerIndex++;
			}
		}

		return;
	}

	Subsystem->PlayerConnected(NewPlayer);

	//If we have no unique net ID we are currently not using online functionality, probably PIE.
	const FUniqueNetIdRepl UniqueNetID = Subsystem->GetUniqueNetIdOf(NewPlayer);

	const FString UniqueNetIDString = UniqueNetID->ToString();

	//This is where a pawn gets spawned if the player doesn't already have one
	Super::PostLogin(NewPlayer);

	// -- HANDLE INACTIVE PLAYER STATES BELOW --

	// Search for inactive states belonging to the connecting players unique player ID.
	for (AOnlinePlayerState* InactiveState : InactiveStates)
	{
		if (InactiveState->UniqueOwnerNetID == UniqueNetIDString)
		{
			InactiveStates.RemoveSingleSwap(InactiveState);
			InactiveState->DispatchCopyProperties(NewPlayer->PlayerState);
			InactiveState->Destroy();
			break;
		}
	}

	//Update the playerstate with the unique player ID, for reconnecting.
	if (auto* CastState = Cast<AOnlinePlayerState>(NewPlayer->PlayerState))
	{
		CastState->UniqueOwnerNetID = UniqueNetIDString;

		if (!NewPlayer->IsLocalPlayerController() && CastState->PlayerIndex <= 0)
		{
			CastState->PlayerIndex = NextPlayerIndex;
			NextPlayerIndex++;
		}
	}

}

void AOnlineGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	// -- REGISTER PAWNS FOR REPOSSESSING WHEN SEAMLESSLY TRAVELLING --

	auto Subsystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();
	if (!Subsystem || !Subsystem->IsPlayerLoggedIn()) return;

	APlayerController* PC = Cast<APlayerController>(C);
	if (!PC || !PC->GetPawn()) return;

	const FUniqueNetIdRepl UniqueNetID = Subsystem->GetUniqueNetIdOf(PC);
	const FString UniqueNetIDString = UniqueNetID->ToString();

	//Update the playerstate with the unique player ID, for reconnecting.
	if (auto* CastState = Cast<AOnlinePlayerState>(C->PlayerState))
	{
		CastState->UniqueOwnerNetID = UniqueNetIDString;

		if (!C->IsLocalPlayerController() && CastState->PlayerIndex <= 0)
		{
			CastState->PlayerIndex = NextPlayerIndex;
			NextPlayerIndex++;
		}
	}
}

void AOnlineGameMode::Logout(AController* Exiting)
{
	if (auto* CastState = Cast<AOnlinePlayerState>(Exiting->PlayerState))
	{
		AOnlinePlayerState* NewPS = GetWorld()->SpawnActor<AOnlinePlayerState>(AOnlinePlayerState::StaticClass());
		CastState->DispatchCopyProperties(NewPS);
		InactiveStates.Add(NewPS);
	}
	Super::Logout(Exiting);
}
