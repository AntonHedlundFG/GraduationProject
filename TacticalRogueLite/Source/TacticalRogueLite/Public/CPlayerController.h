// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlinePlayerController.h"
#include "CPlayerController.generated.h"

class ACUnit;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACPlayerController : public AOnlinePlayerController
{
	GENERATED_BODY()
	
public:

	/** This function is called by client to notify server of object use.
	* on the server, it is forwarded to the GameMode to handle activitiy.
	* @param Unit - The intended using unit. Should be obvious due to unit ordering,
	* but it might be good to verify.
	* @param Slot - Item slot to activate
	* @param TileIndex - Index of the targeted Tile
	* @returns true if ability use was successful
	*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Abilities|Commands")
	void Server_UseObject(ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Abilities|Commands")
	void Server_TryUndo();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Abilities")
	void Server_TryEndTurn();
};
