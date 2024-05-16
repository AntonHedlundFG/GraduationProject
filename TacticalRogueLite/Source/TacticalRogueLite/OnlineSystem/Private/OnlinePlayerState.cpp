// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalRogueLite\OnlineSystem\Public\OnlinePlayerState.h"
#include "Net/UnrealNetwork.h"


void AOnlinePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOnlinePlayerState, PlayerIndex);
}

void AOnlinePlayerState::CopyProperties(APlayerState* PlayerState)
{
	if (!IsValid(PlayerState)) return;

	Super::CopyProperties(PlayerState);

	if (AOnlinePlayerState* CastState = Cast<AOnlinePlayerState>(PlayerState))
	{
		CastState->UniqueOwnerNetID = UniqueOwnerNetID;
		CastState->PlayerIndex = PlayerIndex;
	}
}
