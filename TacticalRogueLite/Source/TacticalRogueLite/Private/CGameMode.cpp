// Fill out your copyright notice in the Description page of Project Settings.

#include "CGameMode.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/CGridTile.h"
#include "CLevelURLAsset.h"
#include "Grid/CGrid.h"
#include "Grid/CGridSpawner.h"
#include "Utility/Logging/CLogManager.h"
#include "Actions/CAction_RollItem.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "Actions/CTargetableAction.h"
#include "Items/CDefaultUnitEquipment.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"
#include "Utility/CRandomComponent.h"
#include "Utility/SaveGame/CSaveGameManager.h"
#include "CUndoAction.h"
#include "Attributes/CAttributeComponent.h"
#include "Achievements/CVictoryCondition.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Actions/CResurrectAction.h"
#include "Grid/CGridRoom.h"

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
		
		GameStateRef->Random->RegisterToSaveManager();
	}
	Spawner = CreateSpawner();
	
	UCSaveGameManager::Get()->LoadGame();


	if (Spawner)
	{
		//Spawn game grid actor
		ACGrid* grid = Spawner->SpawnGrid(FVector::Zero());
		GameStateRef->GameGrid = grid;

		//Spawn initial room with enemies
		Spawner->SpawnRoomWithEnemies(grid, 1, 4, true);
		
		InitializeHeroUnits(grid);
		for ( ACUnit* HeroUnit : HeroUnits)
		{
			HeroUnit->GetGameplayTags().AddTag(TAG_Unit_IsPlayer);
		}
		
		// Testing for multiple room
		//Spawner->SpawnRoomWithEnemies(grid);
		
	}
	AllUnits.Append(HeroUnits);
	AllUnits.Append(EnemyUnits);
	
	//InitializeVictoryCondition();
	
	InitializeTurnOrder(AllUnits);
}

void ACGameMode::RegisterAction(UCAction* inAction)
{
	ActionStack.Add(inAction);
}

UCAction* ACGameMode::RegisterActionOfClass(TSubclassOf<UCAction> inActionClass)
{
	UCAction* NewAction = NewObject<UCAction>(this, inActionClass);
	ActionStack.Add(NewAction);
	return NewAction;
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
	if (!OutAbility.IsValidTargetTile(inUnit->GetTile(), inTargetTile))//ActionComp inte har blockingtags) Playercontroller, här och UI. 
	{
		LOG_WARNING("Target tile is not valid for this item slot");
		return false;
	}

	if (ActionComponent->ActiveGameplayTags.HasAny(OutAbility.BlockingTags))
	{
		LOG_WARNING("Units ActionComponent contains blocking tags from Ability");
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


	TArray<UCAction*> TempActionStack;
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
		TempActionStack.Add(NewAction);
	}
	TempActionStack.Append(ActionStack);
	ActionStack = TempActionStack;

	ExecuteActionStack(inUnit);	

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

		//Broadcast delegate on server since it does not receive OnRep-calls.
		GameStateRef->OnActionListUpdate.Broadcast();
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

	// Execute all actions stored on the stack, if there are any left over, before ending the turn
	ExecuteActionStack();

	//Check for victory conditions
	if (VictoryCondition && VictoryCondition->CheckVictoryCondition())
	{
		if (HandleVictoryConditionMet())
		{
			return true;
		}
		for (auto Unit : HeroUnits)
		{
			if (RollItemClass)
			{
				
				UCAction* Action = RegisterActionOfClass(RollItemClass);
				Action->Initialize(Unit->GetActionComp());
			}
		}
		ExecuteActionStack();
	}

	//Check for loss conditions
	if (VictoryCondition && VictoryCondition->CheckLossCondition())
	{
		//Temporary implementation.
		LOG_GAMEPLAY("You've lost the game!");
		GameStateRef->SetGameIsOver(true);
		return true;
	}

	//Transfer all commands this turn into the command history
	for (UCAction* Action : GameStateRef->ActionList)
	{
		GameStateRef->ActionHistory.Add(Action);
	}
	GameStateRef->ActionList.Empty();
	GameStateRef->OnActionListUpdate.Broadcast();

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

	NextUndoIndex = -1;

	LOG_GAMEPLAY("Turn ended");
	return true;
}

void ACGameMode::ExecuteActionStack(AActor* InstigatingActor)
{
	// Iterate through and execute action stack.
	// Note that the stack -CAN- grow during iteration, as triggered actions can be registered
	// as a result of executed actions.
	int Iterations = 0;
	while (!ActionStack.IsEmpty())
	{
		UCAction* CurrentAction = ActionStack.Pop();
		GameStateRef->ActionList.Add(CurrentAction);
		CurrentAction->StartAction(InstigatingActor);
		
		

		Iterations++;
		if (Iterations > 1000)
		{
			LOG_WARNING("Action Stack tried to execute over 1000 actions. Infinite loop suspected, clearing stack.");
			ActionStack.Empty();
			break;
		}
	}

	GameStateRef->OnActionListUpdate.Broadcast();
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

void ACGameMode::AddEnemyUnits(TArray<ACUnit*> Enemies)
{
	for (ACUnit* EnemyUnit : Enemies)
	{
		EnemyUnit->ControllingPlayerIndex = 0;
		EnemyUnit->GetGameplayTags().AddTag(TAG_Unit_IsEnemy);
		
	}
	
	EnemyUnits.Append(Enemies);
}

void ACGameMode::SpawnDefaultHeroUnits(ACGrid* InGrid)
{
	if (DefaultEquipmentData)
	{
		HeroUnits = Spawner->SpawnUnitsFromArray(Spawner->HeroUnits, InGrid->GetHeroSpawnTiles(), Spawner->HeroNames, FGameplayTag::RequestGameplayTag("Unit.IsPlayer"), TemporaryClassContainer);

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
	
	if (Spawner->SpawnData.Num() != HeroUnitsNum)
	{
		SpawnDefaultHeroUnits(InGrid);
		return;
	}
		
	HeroUnits = TArray<ACUnit*>();

	TArray<ACGridTile*> SpawnTiles = InGrid->GetHeroSpawnTiles();
	for(int8 i = 0; i < HeroUnitsNum ; i++)
	{
		if (i >= Spawner->SpawnData.Num() || i >= SpawnTiles.Num())
			break;
		
		ACUnit* Unit = Spawner->SpawnAndInitializeUnit(Spawner->HeroUnits[i], SpawnTiles[i], Spawner->SpawnData[i], FGameplayTag::RequestGameplayTag("Unit.IsPlayer"));

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

// void ACGameMode::InitializeVictoryCondition()
// {
//
// 	//This is a temporary implementation that should probably be handled by a 
// 	//procedural generation system.
//
// 	VictoryCondition = NewObject<UCVictoryCondition>(this, UCVictoryCondition::StaticClass());
// 	VictoryCondition->Initialize(this, GameStateRef);
// }

void ACGameMode::SetVictoryCondition(UCVictoryCondition* inVictoryCondition)
{
	VictoryCondition = inVictoryCondition;

	if (VictoryCondition && GameStateRef)
	{
		GameStateRef->SetWinConText(VictoryCondition->GetConditionText());
	}
}

bool ACGameMode::HandleVictoryConditionMet()
{
	if (CurrentRoom < RoomsUntilWin)
	{
		if (GetGameGrid() && Spawner)
		{
			CurrentRoom++;
			Spawner->SpawnRoomWithEnemies(GetGameGrid(), CurrentRoom, DefaultEnemyAmount + (CurrentRoom - 1));
			ResurrectAndProgressToNewRoom();
			GetGameGrid()->DestroyOldRooms(this);
		}
		else
		{
			LOG_ERROR("GameMode HandleVictoryConditionMet: Missing Grid or Spawner Reference");
		}
		return false;
	}

	LOG_GAMEPLAY("You've won the game!");
	GameStateRef->SetGameIsOver(true);
	return true;
}

void ACGameMode::ResurrectAndProgressToNewRoom()
{
	TArray<ACGridTile*> HeroTiles = GetGameGrid()->GetLatestRoom()->GetHeroSpawnTiles();
	if (HeroUnits.Num() != HeroTiles.Num())
	{
		LOG_WARNING("Hero Tiles: %d doesn't match Hero Units: %d", HeroTiles.Num(), HeroUnits.Num());
		return;
	}

	for (int i = 0; i < HeroUnits.Num(); i++)
	{
		UCResurrectAction* ResurrectAction;
		if (IsValid(ResurrectType))
			ResurrectAction = NewObject<UCResurrectAction>(this, ResurrectType);
		else
			ResurrectAction = NewObject<UCResurrectAction>(this, UCResurrectAction::StaticClass());

		ResurrectAction->AffectedUnit = HeroUnits[i];
		ResurrectAction->ResurrectOnTile = HeroTiles[i];
		RegisterAction(ResurrectAction);
	}

	ExecuteActionStack();
}
