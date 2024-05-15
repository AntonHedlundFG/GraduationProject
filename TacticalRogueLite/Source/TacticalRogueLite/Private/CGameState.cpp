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

void ACGameState::BeginPlay()
{
	Super::BeginPlay();
	
	// Make sure to set GameSpeed above 0 as to not freeze the game
	GameSpeed = FMath::Max(GameSpeed, 0.1f);

	if (UCLogManager* LogManager = UCLogManager::Get())
	{
		if (GetNetMode() <= ENetMode::NM_ListenServer)
		{
			LogManager->OnServerBroadcastMessage.AddDynamic(this, &ACGameState::Multicast_BroadcastLogMessage);
		}
	}

	GetWorld()->SpawnActor(TurnIndicator);

}

void ACGameState::SetGameSpeed(float NewSpeed)
{
	GameSpeed = FMath::Clamp(NewSpeed, 0.1f, 10.0f);
}

void ACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGameState, CurrentUnit);
	DOREPLIFETIME(ACGameState, TurnOrder);
	DOREPLIFETIME(ACGameState, Random);
	DOREPLIFETIME(ACGameState, ActionList);
	DOREPLIFETIME(ACGameState, ActionHistory);
	DOREPLIFETIME(ACGameState, WinConText);
	DOREPLIFETIME(ACGameState, bGameIsOver);
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

void ACGameState::OnRep_ActionList()
{
	if (!ActionList.IsEmpty() && !IsValid(ActionList[ActionList.Num()-1]))
		return;
	if (!ActionHistory.IsEmpty() && !IsValid(ActionHistory[ActionHistory.Num()-1]))
		return;

	OnActionListUpdate.Broadcast();
}

void ACGameState::Multicast_BroadcastLogMessage_Implementation(ELogCategory Category, const FString& Message)
{
	LOG(Category, "%s", *Message);
}

void ACGameState::SetGameIsOver(bool inbGameIsOver)
{
	if (GetNetMode() > ENetMode::NM_ListenServer) return;

	bGameIsOver = inbGameIsOver;
	OnGameIsOver.Broadcast();
}

void ACGameState::OnRep_WinConText()
{
	if (WinConWidget)
		WinConWidget->UpdateWinConText(WinConText);
}

#pragma region Turn Order

void ACGameState::AddUnitToOrder(ACUnit* inUnit, int32 Index)
{
	if (GetNetMode() == ENetMode::NM_Client) return;

	if (Index < -1 || Index > TurnOrder.Num())
	{
		TurnOrder.Add(inUnit);
		return;
	}
	
	//This should mean someone is undoing the death of the current unit.
	if (Index == -1 && inUnit == CurrentUnit)
	{
		bDoNotAddCurrentBackIntoQueue = false;
		return;
	}

	TurnOrder.Insert(inUnit, Index);
	OnRep_TurnOrder();
}

int32 ACGameState::RemoveUnitFromOrder(ACUnit* inUnit)
{
	if (GetNetMode() == ENetMode::NM_Client) return -2;

	if (inUnit == CurrentUnit)
	{
		bDoNotAddCurrentBackIntoQueue = true;
		return -1;
	}
	int32 Index = TurnOrder.Find(inUnit);
	if (Index == INDEX_NONE) 
		Index = -2;
	else
	{
		TurnOrder.RemoveSingle(inUnit);
		OnRep_TurnOrder();
	}
	return Index;
}

TArray<ACUnit*> ACGameState::GetCurrentTurnOrder(bool bIncludeCurrentUnit)
{
	if (!bIncludeCurrentUnit)
	{
		return TurnOrder;
	}
	TArray<ACUnit*> OrderWithCurrent;
	OrderWithCurrent.Add(CurrentUnit);
	OrderWithCurrent.Append(TurnOrder);
	return OrderWithCurrent;
}

void ACGameState::ProgressToNextTurn()
{
	if (GetNetMode() == ENetMode::NM_Client) return;

	if (bDoNotAddCurrentBackIntoQueue)
	{
		bDoNotAddCurrentBackIntoQueue = false;
	}
	else
	{
		if (IsValid(CurrentUnit))
			TurnOrder.Add(CurrentUnit);
	}

	CurrentUnit = TurnOrder[0];
	TurnOrder.RemoveAt(0);

	OnRep_TurnOrder();
}

void ACGameState::ClearTurnOrder()
{
	CurrentUnit = nullptr;
	TurnOrder.Empty();
	OnRep_TurnOrder();
}

#pragma endregion