// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameState.h"

#include "Utility/CRandomComponent.h"
#include "Net/UnrealNetwork.h"

ACGameState::ACGameState()
{
	Random = CreateDefaultSubobject<UCRandomComponent>(TEXT("Random"));
}

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, TurnOrder);
	DOREPLIFETIME(ACGameState, Random);
}

