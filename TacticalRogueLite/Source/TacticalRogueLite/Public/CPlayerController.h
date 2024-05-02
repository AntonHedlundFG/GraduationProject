// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlinePlayerController.h"
#include "GameplayTagContainer.h"
#include "Actions/CAction_RollItem.h"
#include "UI/CItemSelectionButton.h"
#include "CPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityInitiated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityUsed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUndidAbility);

class ACUnit;
class ACGridTile;
class ACGameState;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACPlayerController : public AOnlinePlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool TryLineCastForGridTile(FVector inStart, FVector Direction, float Distance, ACGridTile*& outTile);
	UFUNCTION(BlueprintCallable)
	bool IsWithEditor() const;
	
private:

	ACGameState* GetGameState();
	ACGameState* GameStateRef;

#pragma region Ability Input

	// Functions in the Ability Input region are to be used by keybindings and UI buttons
public:

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void UndoAbility();

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnUndidAbility OnUndidAbility;

	// This should be called when the player STARTS using an ability, by using a keybind
	// or UI button.
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitiateAbilityUse(FGameplayTag inTag);

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityInitiated OnAbilityInitiated;

	// This should be called when a player targets a tile after having used an ability
	// through InitiateAbilityUse. This function makes the actual server RPC.
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void FinalizeAbilityUse(ACGridTile* inTargetTile);

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityUsed OnAbilityUsed;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void CancelAbilityUse();

	UFUNCTION(BlueprintCallable, Category = "Turn Order")
	void EndTurn();

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnTurnEnded OnTurnEnded;

protected:

	// Stores the currently used itemslot designated in InitiateAbilityUse()
	EItemSlots ItemSlotCurrentlyUsed = EItemSlots::EIS_None;
	FGameplayTag TagCurrentlyUsed;
	// Stores the currently active Unit, fetched from the fromt of the turn order
	// in InitiateAbilityUse().
	// It's useful to make sure you don't try to initiate an ability, then end turn, 
	// then finalize.
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	ACUnit* UnitCurrentlyUsingAbility = nullptr;
	UPROPERTY()
	ACGridTile* TileCurrentlyTargeted = nullptr;

	TArray<ACGridTile*> HighlightedTiles;

#pragma endregion

#pragma region Server RPCs

	/** This function is called by client to notify server of object use.
	* on the server, it is forwarded to the GameMode to handle activitiy.
	* @param Unit - The intended using unit. Should be obvious due to unit ordering,
	* but it might be good to verify.
	* @param Slot - Item slot to activate
	* @param TileIndex - Index of the targeted Tile
	* @returns true if ability use was successful
	*/
	UFUNCTION(Server, Reliable, Category = "Abilities|Commands")
	void Server_UseObject(ACUnit* inUnit, FGameplayTag inTag, ACGridTile* inTargetTile);
	
	UFUNCTION(Server, Reliable, Category = "Abilities|Commands")
	void Server_TryUndo();

	UFUNCTION(Server, Reliable, Category = "Abilities")
	void Server_TryEndTurn();
	
public:
	UFUNCTION(Server, Reliable, Category = "Abilities|Commands")
	void Server_EquipItem(ACUnit* inUnit, UCItemData* Item,UCAction_RollItem* Action);

#pragma endregion

};
