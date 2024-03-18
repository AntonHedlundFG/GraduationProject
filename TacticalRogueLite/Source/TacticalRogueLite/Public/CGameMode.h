// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "CGameMode.generated.h"

class ACUnit;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameMode : public AOnlineGameMode
{
	GENERATED_BODY()
	
public:
	/** Should be called by a controller (AI or Player) to attempt an ability use.
	* @param Controller - The source of the ability activation
	* @param Unit - The intended using unit. Should be obvious due to unit ordering,
	* but it might be good to verify.
	* @param Slot - Item slot to activate
	* @param TileIndex - Index of the targeted Tile
	* @returns true if ability use was successful
	*/
	bool TryAbilityUse(AController* inController, ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex);

};
