// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameState.h"
#include "Net/UnrealNetwork.h"

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, TurnOrder);
}
