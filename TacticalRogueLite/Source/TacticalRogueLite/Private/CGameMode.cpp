// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Grid\CGridTile.h"
#include "CLevelURLAsset.h"
#include "Grid/CGrid.h"
#include "Grid/CGridSpawner.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "Actions/CTargetableAction.h"
#include "Items/CDefaultUnitEquipment.h"
#include "Items/CNamesAndItemsList.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"
#include "Utility/CRandomComponent.h"
//#include "Settings/LevelEditorPlaySettings.h"
#include "Utility/SaveGame/CSaveGameManager.h"
#include "CUndoAction.h"
#include "Attributes/CAttributeComponent.h"
#include "Achievements\CVictoryCondition.h"
#include "Utility/SaveGame/CSaveGame.h"

void ACGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			LOG_WARNING("Wrong Game State class");
			return;
		}
	}
	Spawner = CreateSpawner();
	
	UCSaveGameManager::Get()->LoadGame();

	ACGrid* grid = Spawner->SpawnGrid(FVector::Zero(),10,10);
	GameStateRef->GameGrid = grid;

	if (Spawner)
	{
		InitializeHeroUnits(grid);
		
		EnemyUnits = Spawner->SpawnUnitsFromArray(Spawner->EnemyUnits, grid->GetEnemySpawnTiles(), Spawner->EnemyNames);
		for (ACUnit* EnemyUnit : EnemyUnits)
		{
			EnemyUnit->ControllingPlayerIndex = 0;
		}
	}
	AllUnits.Append(HeroUnits);
	AllUnits.Append(EnemyUnits);

	if (DefaultEquipmentData)
	{
		for (auto Unit : EnemyUnits)
		{
			DefaultEquipmentData->EquipUnit(Unit);
		}
	}
	else
	{
		LOG_WARNING("DefaultUnitEquipmentData missing in GameMode");
	}
	
	InitializeVictoryCondition();

	InitializeTurnOrder(AllUnits);
}

void ACGameMode::RegisterAction(UCAction* inAction)
{
	ActionStack.Add(inAction);
}

void ACGameMode::RegisterActionOfClass(TSubclassOf<UCAction> inActionClass)
{
	UCAction* NewAction = NewObject<UCAction>(this, inActionClass);
	ActionStack.Add(NewAction);
}

bool ACGameMode::TryAbilityUse(AController* inController, ACUnit* inUnit, FGameplayTag inItemSlotTag, ACGridTile* inTargetTile)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			LOG_WARNING("Wrong Game State class");
			return false;
		}
	}

	if (GameStateRef->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return false;
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		LOG_WARNING("There are no units in the turn order");
		return false;
	}

	if (GameStateRef->TurnOrder[0] != inUnit)
	{
		LOG_WARNING("Unit is not first in the turn order");
		return false;
	}

	if (!inUnit->IsControlledBy(inController))
	{
		LOG_WARNING("Unit is not owned by controller");
		return false;
	}

	UCActionComponent* ActionComponent = inUnit->GetActionComp();
	if (!ActionComponent)
	{
		LOG_WARNING("No Action Component on unit");
		return false;
	}

	FAbility OutAbility;
	ActionComponent->TryGetAbility(inItemSlotTag, OutAbility);
	if (!OutAbility.IsValidTargetTile(inUnit->GetTile(), inTargetTile))
	{
		LOG_WARNING("Target tile is not valid for this item slot");
		return false;
	}

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(inUnit);
	if (!Attributes)
	{
		LOG_WARNING("No Attribute Component on unit");
		return false;
	}
	if (!Attributes->TrySpendCharge(inItemSlotTag))
	{
		LOG_WARNING("Not enough charges remaining on item");
		return false;
	}
	

	// Create instances of each action in the used ability, and add them to the stack in reverse order
	for (int i = OutAbility.Actions.Num() - 1; i >= 0; i--)
	{
		UCAction* NewAction = NewObject<UCAction>(ActionComponent, OutAbility.Actions[i]);

		if (i == 0)
		{
			NewAction->bIsUserIncited = true;
			NewAction->UserIncitedItemSlot = inItemSlotTag;
		}

		NewAction->Initialize(ActionComponent);
		UCTargetableAction* Targetable = Cast<UCTargetableAction>(NewAction);
		if (Targetable)
		{
			Targetable->TargetTile = inTargetTile;
		}
		ActionStack.Add(NewAction);
	}

	// Now that the stack is full of actions, start iterating through and executing them.
	// Note that the stack -CAN- grow during iteration, as triggered actions can be registered
	// as a result of executed actions.
	int Iterations = 0;
	while (!ActionStack.IsEmpty())
	{
		UCAction* CurrentAction = ActionStack.Pop();
		GameStateRef->ActionList.Add(CurrentAction);
		CurrentAction->StartAction(inUnit);
		
		Iterations++;
		if (Iterations > 1000)
		{
			LOG_WARNING("Action Stack tried to execute over 1000 actions. Infinite loop suspected, clearing stack.");
			ActionStack.Empty();
			break;
		}
	}
	GameStateRef->OnActionListUpdate.Broadcast();

	//We update the UndoIndex since we know this action was triggered by player input.
	NextUndoIndex = GameStateRef->ActionList.Num() - 1;

	return true;
}

bool ACGameMode::TryUndo(AController* inController)
{
	if (!GameStateRef || !GameStateRef->TurnOrder[0])
	{
		LOG_WARNING("No valid unit at front of game state turn order");
		return false;
	}

	if (GameStateRef->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return false;
	}

	uint8 PlayerIndex = GameStateRef->TurnOrder[0]->ControllingPlayerIndex;
	AOnlinePlayerState* PlayerState = inController->GetPlayerState<AOnlinePlayerState>();
	if (!PlayerState || PlayerState->PlayerIndex != PlayerIndex)
	{
		LOG_WARNING("Controller does not control current unit");
		return false;
	}

	//Here we establish a list of all actions that should be undone in the same batch
	//as many of them will NOT be UserIncited. When we've found a userIncited action, 
	//we finish our undoing.
	TArray<TSoftObjectPtr<UCAction>> UndoneActions;
	while (NextUndoIndex >= 0 && !GameStateRef->ActionList[NextUndoIndex]->bIsUndone)
	{
		UCAction* CurrentAction = GameStateRef->ActionList[NextUndoIndex];
		CurrentAction->UndoAction(inController);
		UndoneActions.Add(CurrentAction);
		NextUndoIndex--;
		if (CurrentAction->bIsUserIncited)
		{
			UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(GameStateRef->TurnOrder[0]);
			if (Attributes)
			{
				Attributes->TryUndoSpendCharge(CurrentAction->UserIncitedItemSlot);
			}
			break;
		}
	}

	//Create the actual UndoAction for replication and visualisation purposes. 
	if (UndoneActions.Num() > 0)
	{
		UCUndoAction* UndoAction = NewObject<UCUndoAction>(this, UCUndoAction::StaticClass());
		UndoAction->UndoneActions = UndoneActions;
		GameStateRef->ActionList.Add(UndoAction);
	}

	//Move our NextUndoIndex pointer to the most recent action which has not been undone.
	while (NextUndoIndex >= 0 && GameStateRef->ActionList[NextUndoIndex]->bIsUndone)
		NextUndoIndex--;

	//Now we validate that there is actually a user incited action that can be undone
	//to make sure the user doesn't undo triggered actions that are created at the start
	//of their turn. Only UserIncited actions and the actions they trigger are undoable.
	int TestUserInciteIndex = NextUndoIndex;
	while (TestUserInciteIndex >= 0 && !GameStateRef->ActionList[TestUserInciteIndex]->bIsUserIncited)
	{
		TestUserInciteIndex--;
	}
	if (TestUserInciteIndex < 0)
		NextUndoIndex = -1;

	//Broadcast delegate on server since it does not receive OnRep-calls.
	GameStateRef->OnActionListUpdate.Broadcast();
	
	return true;
}

bool ACGameMode::TryEndTurn(AController* inController)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			LOG_WARNING("Wrong Game State class");
			return false;
		}
	}

	if (GameStateRef->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return false;
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		LOG_WARNING("There are no units in the turn order");
		return false;
	}

	ACUnit* CurrentUnit = GameStateRef->TurnOrder[0];
	if (!CurrentUnit->IsControlledBy(inController))
	{
		LOG_WARNING("Controller does not manage current unit");
		return false;
	}

	//Check for victory conditions
	if (VictoryCondition && VictoryCondition->CheckVictoryCondition())
	{
		//Temporary implementation.
		LOG_GAMEPLAY("You've won the game!");
		GameStateRef->SetGameIsOver(true);
		return true;
	}

	//Check for loss conditions
	if (VictoryCondition && VictoryCondition->CheckLossCondition())
	{
		//Temporary implementation.
		LOG_GAMEPLAY("You've lost the game!");
		GameStateRef->SetGameIsOver(true);
		return true;
	}

	//Move active unit to back of the line
	GameStateRef->TurnOrder.RemoveAt(0);
	GameStateRef->TurnOrder.Add(CurrentUnit);
	GameStateRef->OnRep_TurnOrder();

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(GameStateRef->TurnOrder[0]);
	if (Attributes)
	{
		LOG_INFO("Resetting spent charges on %s", *GameStateRef->TurnOrder[0]->GetUnitName());
		Attributes->ResetSpentCharges();
	}

	//Move TurnTimerSubsystem forward
	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
	{
		Subsystem->NextTurn(CurrentUnit, GameStateRef->TurnOrder[0]);
	}

	//Transfer all commands this turn into the command history
	for (UCAction* Action : GameStateRef->ActionList)
	{
		GameStateRef->ActionHistory.Add(Action);
	}
	GameStateRef->ActionList.Empty();
	GameStateRef->OnActionListUpdate.Broadcast();

	NextUndoIndex = -1;

	LOG_GAMEPLAY("Turn ended");
	return true;
}

void ACGameMode::InitializeTurnOrder(const TArray<ACUnit*>& Units)
{
	GameStateRef->TurnOrder.Empty();
	
	TArray<ACUnit*> UnitsRemaining = Units;
	while (UnitsRemaining.Num() > 0)
	{
		int RandomIndex = GameStateRef->Random->GetRandRange(0, UnitsRemaining.Num() - 1, false);
		GameStateRef->AddUnitToOrder(UnitsRemaining[RandomIndex]);
		UnitsRemaining.RemoveAtSwap(RandomIndex);
	}
	GameStateRef->OnRep_TurnOrder();
}

void ACGameMode::ApplyPlayerCount(const TArray<ACUnit*>& Units)
{
	int PlayerCount = UGameplayStatics::GetIntOption(OptionsString, NUMBER_OF_PLAYERS, DefaultPlayerCount);

	for (ACUnit* Unit : Units)
	{
		if (!Unit) continue;

		// Might look tricky, but it's fairly simple. 
		// If player count is 2, characters with 3/4 become 1/2 instead.
		while (Unit->ControllingPlayerIndex > PlayerCount)
		{
			Unit->ControllingPlayerIndex -= PlayerCount;
		}
	}
}

ACGridSpawner* ACGameMode::CreateSpawner()
{
	ACGridSpawner* spawner = GetWorld()->SpawnActor<ACGridSpawner>(SpawnerClass, FVector::Zero(), FRotator::ZeroRotator);
	return spawner;
}

void ACGameMode::SpawnDefaultHeroUnits(ACGrid* InGrid)
{
	if (DefaultEquipmentData)
	{
		HeroUnits = Spawner->SpawnUnitsFromArray(Spawner->HeroUnits, InGrid->GetHeroSpawnTiles(), Spawner->HeroNames);

		for (auto Unit : HeroUnits)
		{
			DefaultEquipmentData->EquipUnit(Unit);
		}
		ApplyPlayerCount(HeroUnits);
	}
	else
	{
		LOG_WARNING("No Default Equipment Data in GameMode");
	}
}

void ACGameMode::InitializeHeroUnits(ACGrid* InGrid)
{
	const int HeroUnitsNum = 4;
	
	UCSaveGame* SaveGame = nullptr;
	if(!UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		LOG_WARNING("Couldn't Find SaveGame When Initializing Units in GameMode");
		SpawnDefaultHeroUnits(InGrid);
		return;
	}
	
	if (Spawner->NamesAndItemList.Num() != HeroUnitsNum)
	{
		SpawnDefaultHeroUnits(InGrid);
		return;
	}
	
	HeroUnits = TArray<ACUnit*>();

	TArray<ACGridTile*> SpawnTiles = InGrid->GetHeroSpawnTiles();
	for(int8 i = 0; i < HeroUnitsNum ; i++)
	{
		if (i >= Spawner->NamesAndItemList.Num() || i >= SpawnTiles.Num())
			break;
		
		ACUnit* Unit = Spawner->SpawnAndInitializeUnit(Spawner->HeroUnits[i], SpawnTiles[i],
							Spawner->NamesAndItemList[i].Items, Spawner->NamesAndItemList[i].Name);

		if (i < Spawner->ControllingPlayers.Num())
		{
			Unit->ControllingPlayerIndex = Spawner->ControllingPlayers[i];
		}
		else
		{
			Unit->ControllingPlayerIndex = 1;
		}
		
		HeroUnits.Add(Unit);
	}
}

void ACGameMode::InitializeVictoryCondition()
{

	//This is a temporary implementation that should probably be handled by a 
	//procedural generation system.

	VictoryCondition = NewObject<UCVictoryCondition>(this, UCVictoryCondition::StaticClass());
	VictoryCondition->Initialize(this, GameStateRef);
}
