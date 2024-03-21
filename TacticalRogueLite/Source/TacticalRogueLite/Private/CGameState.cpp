// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameState.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, TurnOrder);
}

void ACGameState::BeginPlay()
{
	Super::BeginPlay();
	Random.Init(0);
}

int32 ACGameState::GetRandom(int32 Min, int32 Max)
{
	// If we are the server, we can use the random stream directly
	if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Server"));
		return Random.GetRandRange(Min, Max, false);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Client"));
	return PeekRandom(Min, Max);
}

int32 ACGameState::PeekRandom(int32 Min, int32 Max)
{
	return Random.GetRandRange(Min, Max, true);
}

int32 ACGameState::RollBackRandom(int32 Ticks)
{
	if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		return Random.RollBackRandom(Ticks);
	}
	return -1;
}

void ACGameState::SaveSeedState()
{
	Random.SaveState();
}
