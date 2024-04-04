// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "CGameMode.generated.h"

class UCDefaultUnitEquipment;
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

	/* Whenever an action is triggered indirectly (not as part of an ability use), register it here.
	* For example: A unit dies as a result of an attack.
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	void RegisterAction(UCAction* inAction);

	// It is preferable to use RegisterAction, so that the caller can have
	// control over object creation, and call inAction->Initialize()
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	void RegisterActionOfClass(TSubclassOf<UCAction> inActionClass);

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
	UFUNCTION(BlueprintCallable, Category = "Units")
	TArray<ACUnit*> GetAllUnits() const { return AllUnits; }
	UFUNCTION(BlueprintCallable, Category = "Units")
	TArray<ACUnit*> GetHeroUnits() const { return HeroUnits; }
	UFUNCTION(BlueprintCallable, Category = "Units")
	TArray<ACUnit*> GetEnemyUnits() const { return EnemyUnits; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACGameState> GameStateRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid|Spawner")
	TSubclassOf<ACGridSpawner> SpawnerClass;
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Spawner")
	TObjectPtr<ACGridSpawner> Spawner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units|DefaultEquipment")
	TObjectPtr<UCDefaultUnitEquipment> DefaultEquipmentData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> AllUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> HeroUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units")
	TArray<ACUnit*> EnemyUnits;

	//Actions placed on stack have not been executed yet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	TArray<UCAction*> ActionStack;

	UFUNCTION(Category = "Grid|Spawner")
	ACGridSpawner* CreateSpawner();

	// If playing in PIE mode, this is how many players we assume are playing.
	UPROPERTY(EditAnywhere, Category = "Units")
	int DefaultPlayerCount = 2;
	
};
