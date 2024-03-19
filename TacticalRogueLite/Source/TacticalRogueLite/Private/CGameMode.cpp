// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "CommandPattern\CCommand.h"
#include "GridContent/CUnit.h"
#include "Items/CItem.h"
#include "CGameState.h"


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
	if (inUnit->GetOwner() != inController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is not owned by controller"));
		return false;
	}

	// TODO: Here we should fetch the actual item from the unit based on the inSlot
	// we need something like inUnit->GetItemInSlot(inSlot)
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

	NewCommand->ExecuteCommand();
	CommandHistory.Add(NewCommand);
	return true;
}

bool ACGameMode::TryUndo(AController* inController)
{
	if (CommandHistory.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No commands in history"));
		return false;
	}
	UCCommand* LastCommand = CommandHistory.Last();
	LastCommand->UndoCommand();
	CommandHistory.RemoveAtSwap(CommandHistory.Num() - 1);
	return true;
}
