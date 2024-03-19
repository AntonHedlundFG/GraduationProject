// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "CGameMode.generated.h"

class ACUnit;
class ACGameState;
class UCCommand;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameMode : public AOnlineGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	/** Should be called by a controller (AI or Player) to attempt an ability use.
	* @param Controller - The source of the ability activation
	* @param Unit - The intended using unit. Should be obvious due to unit ordering,
	* but it might be good to verify.
	* @param Slot - Item slot to activate
	* @param TileIndex - Index of the targeted Tile
	* @returns true if ability use was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	bool TryAbilityUse(AController* inController, ACUnit* inUnit, const EItemSlots inSlot, const int inTileIndex);

	// Attempts to undo the latest command if it was created by inController
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	bool TryUndo(AController* inController);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryEndTurn(AController* inController);

	//Resets game state turn order list, and adds all Units in world to the order list.
	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	void InitializeTurnOrder();

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACGameState> GameStateRef;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Commands")
	TArray<UCCommand*> CommandList;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Commands")
	TArray<UCCommand*> CommandHistory;
};
