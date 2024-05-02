// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "Items\ItemSlots.h"
#include "TacticalRogueLite\OnlineSystem\Public\OnlineGameMode.h"
#include "CGameMode.generated.h"

class UCSavedUnitAndItemData;
class UCDefaultUnitEquipment;
class ACGridSpawner;
class ACGrid;
class ACUnit;
class ACGameState;
class UCCommand;
class ACGridTile;
class UCConsequence;
class UCAction;
class UCVictoryCondition;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACGameMode : public AOnlineGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:

	//Resets game state turn order list, and adds all Units in world to the order list.
	void InitializeTurnOrder(const TArray<ACUnit*>& Units);

	//If fewer than 4 players, this function "downgrades" units, until they are
	//considered controlled by an actual player.
	//With 2 players, unit 3/4 become controlled by 1/2.
	//With 3 players, unit 4 become controlled by 1
	//With 1 player, all units become controlled by 1.
	void ApplyPlayerCount(const TArray<ACUnit*>& Units);

	UFUNCTION(Category = "Units")
	void InitializeHeroUnits(ACGrid* InGrid);

	// If playing in PIE mode, this is how many players we assume are playing.
	UPROPERTY(EditAnywhere, Category = "Units")
	int DefaultPlayerCount = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Units|Defaults")
	TObjectPtr<UCDefaultUnitEquipment> DefaultEquipmentData;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACGameState> GameStateRef;


#pragma region Actions & Abilities

public:

	/* Whenever an action is triggered indirectly (not as part of an ability use), register it here.
	* For example: A unit dies as a result of an attack.
	*/
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	void RegisterAction(UCAction* inAction);

	// It is preferable to use RegisterAction, so that the caller can have
	// control over object creation, and call inAction->Initialize()
	UFUNCTION(BlueprintCallable, Category = "Abilities|Commands")
	UCAction* RegisterActionOfClass(TSubclassOf<UCAction> inActionClass);

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

protected:

	//Actions placed on stack have not been executed yet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	TArray<UCAction*> ActionStack;

	//This keeps track of which index in GameState->ActionList should be the starting point
	//of undoing actions when TryUndo is successfully called.
	UPROPERTY()
	int NextUndoIndex = -1;

	void ExecuteActionStack(AActor* InstigatingActor = nullptr);

#pragma endregion



#pragma region Grid and Content

public:

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
	UFUNCTION(BlueprintCallable, Category = "Units")
	void AddEnemyUnits(TArray<ACUnit*> Enemies);


protected:
	///Create grid spawner to handle room and unit spawning
	UFUNCTION(Category = "Grid|Spawner")
	ACGridSpawner* CreateSpawner();
	///Spawn hero units with default items
	UFUNCTION(Category = "Units")
	void SpawnDefaultHeroUnits(ACGrid* InGrid);

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

#pragma endregion



#pragma region Victory Conditions

public:

	UFUNCTION()
	UCVictoryCondition* GetVictoryCondition() const { return VictoryCondition; }
	UFUNCTION()
	void SetVictoryCondition(UCVictoryCondition* inVictoryCondition) { VictoryCondition = inVictoryCondition; }
	
	UPROPERTY(EditDefaultsOnly, Category = "Rolling Item Class")
	TSubclassOf<UCAction> RollItemClass;
	
protected:

	//This class represents the victory (and loss) conditions. 
	//Right now we create it right here in the game mode, but it should 
	//be assigned to the game mode by the procedural generation system later.
	//At the end of each turn, we call the CheckVictoryCondition() and 
	//CheckLossCondition() of this object.
	UPROPERTY(BlueprintReadOnly, Category = "Victory Condition")
	TObjectPtr<UCVictoryCondition> VictoryCondition;

	//This is run in BeginPlay to assign relevant references to the victory condition.
	//Once we have a procedural generation system, it should probably take over this task.
	UFUNCTION(Category = "Victory Condition")
	void InitializeVictoryCondition();
	///Handles fulfilling the victory condition in a given room, returns true if entire game is won
	UFUNCTION(Category = "Victory Condition")
	bool HandleVictoryConditionMet();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory Condition")
	int RoomsUntilWin = 4;
	UPROPERTY()
	int CurrentRoom = 1;

	void ResurrectAndProgressToNewRoom();
	
#pragma endregion
	
};
