// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "CommandPattern\CCommand.h"
#include "Items\CItemAbility.h"

bool ACGameMode::TryAbilityUse(AController* inController, ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex)
{
	//Validate that the player controller is in control of the current unit.

	//We should be able to fetch this reference by checking which unit is currently selected
	UCItemAbility* ItemAbility = nullptr; // = GetActiveUnit()->GetAbilityInSlot(Slot); or something

	//Verify that the target Tile is a reasonable target. 
	//Maybe the ability should have a function for this?

	UCCommand* NewCommand = ItemAbility->GenerateCommand();
	NewCommand->ExecuteCommand();

	return true;
}
