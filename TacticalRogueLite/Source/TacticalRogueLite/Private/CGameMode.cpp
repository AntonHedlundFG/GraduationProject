// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "CommandPattern\CCommand.h"
#include "GridContent/CUnit.h"
#include "Items/CItem.h"
#include "CGameState.h"
#include "Kismet/GameplayStatics.h"

void ACGameMode::BeginPlay()
{
	//We might want to move this to another place later, 
	// as all Units might not be spawned yet.
	InitializeTurnOrder();
}

bool ACGameMode::TryAbilityUse(AController* inController, ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return false;
		}
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no units in the turn order"));
		return false;
	}

	if (GameStateRef->TurnOrder[0] != inUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is not first in the turn order"));
		return false;
	}

	if (!inUnit->IsControlledBy(inController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is not owned by controller"));
		return false;
	}

	UCItem* Item = inUnit->GetItemInSlot(inSlot);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("No item in this item slot"));
		return false;
	}

	UCCommand* NewCommand = Item->GenerateAbilityCommand(inController, inUnit, inTileIndex);
	if (!NewCommand)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability command on this item"));
		return false;
	}

	//Verify that the target Tile is a reasonable target. 
	//Maybe the ability should have a function for this?
	// if (!ItemAbility->IsValidTarget(Grid->GetTileAt(inTileIndex)){} or something

	NewCommand->ExecuteCommand(inController);
	CommandList.Add(NewCommand);

	return true;
}

bool ACGameMode::TryUndo(AController* inController)
{
	if (CommandList.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No commands in history"));
		return false;
	}

	UCCommand* LastCommand = CommandList.Last();
	if (LastCommand->GetCommandCreator() != inController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Command was not performed by this controller"));
		return false;
	}

	LastCommand->UndoCommand();
	CommandList.RemoveAtSwap(CommandList.Num() - 1);

	return true;
}

bool ACGameMode::TryEndTurn(AController* inController)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return false;
		}
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no units in the turn order"));
		return false;
	}

	ACUnit* CurrentUnit = GameStateRef->TurnOrder[0];
	if (!CurrentUnit->IsControlledBy(inController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller does not manage current unit"));
		return false;
	}

	//Move active unit to back of the line
	GameStateRef->TurnOrder.RemoveAt(0);
	GameStateRef->TurnOrder.Add(CurrentUnit);

	//Transfer all commands this turn into the command history
	for (UCCommand* Command : CommandList)
	{
		CommandHistory.Add(Command);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Command->ToString());
	}
	CommandList.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Turn ended"));
	return true;
}

void ACGameMode::InitializeTurnOrder()
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return;
		}
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACUnit::StaticClass(), OutActors);

	GameStateRef->TurnOrder.Empty();
	for (AActor* Actor : OutActors)
	{
		ACUnit* Unit = Cast<ACUnit>(Actor);
		if (Unit)
			GameStateRef->AddUnitToOrder(Unit);
	}
}