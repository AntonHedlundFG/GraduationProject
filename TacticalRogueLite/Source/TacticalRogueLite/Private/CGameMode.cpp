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
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

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

	ACGrid* grid = Spawner->SpawnGrid(FVector::Zero(),10,10);
	GameStateRef->GameGrid = grid;

	if (Spawner)
	{
		HeroUnits = Spawner->SpawnUnitsFromArray(Spawner->HeroUnits, grid->GetHeroSpawnTiles(), Spawner->HeroNames);
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
		for (auto Unit : AllUnits)
		{
			DefaultEquipmentData->EquipUnit(Unit);
		}
	}
	else
	{
		LOG_WARNING("DefaultUnitEquipmentData missing in GameMode");
	}
	
	InitializeTurnOrder(AllUnits);
	ApplyPlayerCount(AllUnits);
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
	
	bool bHasDesignatedIncitingAction = false;

	for (int i = OutAbility.Actions.Num() - 1; i >= 0; i--)
	{
		UCAction* NewAction = NewObject<UCAction>(ActionComponent, OutAbility.Actions[i]);

		if (i == 0)
			NewAction->bIsUserIncited = true;

		NewAction->Initialize(ActionComponent);
		UCTargetableAction* Targetable = Cast<UCTargetableAction>(NewAction);
		if (Targetable)
		{
			Targetable->TargetTile = inTargetTile;
		}
		ActionStack.Add(NewAction);
	}

	int Iterations = 0;
	while (!ActionStack.IsEmpty())
	{
		UCAction* CurrentAction = ActionStack.Pop();
		ActionList.Add(CurrentAction);
		CurrentAction->StartAction(inUnit);
		
		Iterations++;
		if (Iterations > 1000)
		{
			LOG_WARNING("Action Stack tried to execute over 1000 actions. Infinite loop suspected, clearing stack.");
			ActionStack.Empty();
			break;
		}
	}

	return true;
}

bool ACGameMode::TryUndo(AController* inController)
{
	if (!GameStateRef || !GameStateRef->TurnOrder[0])
	{
		LOG_WARNING("No valid unit at front of game state turn order");
		return false;
	}

	uint8 PlayerIndex = GameStateRef->TurnOrder[0]->ControllingPlayerIndex;
	AOnlinePlayerState* PlayerState = inController->GetPlayerState<AOnlinePlayerState>();
	if (!PlayerState || PlayerState->PlayerIndex != PlayerIndex)
	{
		LOG_WARNING("Controller does not control current unit");
		return false;
	}

	while (true)
	{
		if (ActionList.IsEmpty())
		{
			LOG_WARNING("No commands in history");
			return false;
		}

		UCAction* LastAction = ActionList.Last();
		LastAction->UndoAction(inController);
		ActionList.RemoveAtSwap(ActionList.Num() - 1);
		if (LastAction->bIsUserIncited)
			break;
	}
	
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

	//Move active unit to back of the line
	GameStateRef->TurnOrder.RemoveAt(0);
	GameStateRef->TurnOrder.Add(CurrentUnit);
	GameStateRef->OnRep_TurnOrder();

	auto* Subsystem = GetWorld()->GetSubsystem<UCTurnTimerSubsystem>();
	if (Subsystem)
	{
		Subsystem->NextTurn(CurrentUnit, GameStateRef->TurnOrder[0]);
	}

	//Transfer all commands this turn into the command history
	for (UCAction* Action : ActionList)
	{
		ActionHistory.Add(Action);
	}
	ActionList.Empty();

	LOG_GAMEPLAY("Turn ended");
	return true;
}

void ACGameMode::InitializeTurnOrder(const TArray<ACUnit*>& Units)
{
	GameStateRef->TurnOrder.Empty();
	for (ACUnit* Unit : Units)
	{
		if (Unit)
			GameStateRef->AddUnitToOrder(Unit);
	}
	GameStateRef->OnRep_TurnOrder();
}

void ACGameMode::ApplyPlayerCount(const TArray<ACUnit*>& Units)
{
	int PlayerCount = UGameplayStatics::GetIntOption(OptionsString, NUMBER_OF_PLAYERS, 0);
	if (!PlayerCount) return;
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
