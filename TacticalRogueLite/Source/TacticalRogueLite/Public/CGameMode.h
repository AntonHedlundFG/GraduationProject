// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "CGameMode.generated.h"

class ACGridSpawner;
class ACGrid;
class ACUnit;
class ACGameState;
class UCCommand;
class ACGridTile;
class UCConsequence;
class UCAction;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameMode : public AOnlineGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	/* Whenever an ability or event is triggered as a result of a UCCommand occuring
	* a UCConsequence should be created and sent into this function for execution.
	* This will make sure it can be undone if a player tries to undo the original UCCommand.
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	void RegisterAndExecuteConsequence(UCConsequence* inConsequence);

	/** Should be called by a controller (AI or Player) to attempt an ability use.
	* @param Controller - The source of the ability activation
	* @param Unit - The intended using unit. Should be obvious due to unit ordering,
	* but it might be good to verify.
	* @param Slot - Item slot to activate
	* @param TileIndex - Index of the targeted Tile
	* @returns true if ability use was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	bool TryAbilityUse(AController* inController, ACUnit* inUnit, FGameplayTag inItemSlotTag, ACGridTile* inTargetTile);

	// Attempts to undo the latest command if it was created by inController
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	bool TryUndo(AController* inController);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryEndTurn(AController* inController);

	//Resets game state turn order list, and adds all Units in world to the order list.
	void InitializeTurnOrder(const TArray<ACUnit*>& Units);

	//If fewer than 4 players, this function "downgrades" units, until they are
	//considered controlled by an actual player.
	//With 2 players, unit 3/4 become controlled by 1/2.
	//With 3 players, unit 4 become controlled by 1
	//With 1 player, all units become controlled by 1.
	void ApplyPlayerCount(const TArray<ACUnit*>& Units);

	UFUNCTION(BlueprintCallable, Category = "Grid|Grid")
	ACGrid* GetGameGrid() const { return GameStateRef->GameGrid; }
	UFUNCTION(BlueprintCallable, Category = "Grid|Spawner")
	ACGridSpawner* GetGridSpawner() const { return Spawner; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACGameState> GameStateRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid|Spawner")
	TSubclassOf<ACGridSpawner> SpawnerClass;
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Spawner")
	TObjectPtr<ACGridSpawner> Spawner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> AllUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> HeroUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> EnemyUnits;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Commands")
	TArray<UCCommand*> CommandList;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Commands")
	TArray<UCCommand*> CommandHistory;

	TArray<UCAction*> ActionList;
	TArray<UCAction*> ActionHistory;

	UFUNCTION(Category = "Grid|Spawner")
	ACGridSpawner* CreateSpawner();
	
};
