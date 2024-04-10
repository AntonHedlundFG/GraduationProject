// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameState.h"
#include "Engine/ActorChannel.h"
#include "Utility/CRandomComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actions/CAction.h"
#include "Actions\Visualizer\CActionVisualizerSystem.h"

ACGameState::ACGameState()
{
	Random = CreateDefaultSubobject<UCRandomComponent>(TEXT("Random"));
	ActionVisualizerSystem = CreateDefaultSubobject<UCActionVisualizerSystem>(TEXT("ActionVisualizerSystem"));
}

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, TurnOrder);
	DOREPLIFETIME(ACGameState, Random);
	DOREPLIFETIME(ACGameState, ActionList);
	DOREPLIFETIME(ACGameState, ActionHistory);
}

bool ACGameState::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UCAction* Action : ActionList)
	{
		if (Action)
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);

	}
	for (UCAction* Action : ActionHistory)
	{
		if (Action)
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);

	}

	return WroteSomething;
}

void ACGameState::SetGameIsOver(bool inbGameIsOver)
{
	if (GetNetMode() > ENetMode::NM_ListenServer) return;

	bGameIsOver = inbGameIsOver;
	OnGameIsOver.Broadcast();
}
