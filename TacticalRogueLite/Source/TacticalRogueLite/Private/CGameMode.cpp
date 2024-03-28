// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"
#include "CommandPattern\CCommand.h"
#include "GridContent/CUnit.h"
#include "Items/CItem.h"
#include "CGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Grid\CGridTile.h"
#include "CLevelURLAsset.h"
#include "Grid/CGrid.h"
#include "Grid/CGridSpawner.h"
#include "CommandPattern/CConsequence.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "Actions/CTargetableAction.h"

void ACGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return;
		}
	}

	Spawner = CreateSpawner();

	ACGrid* grid = Spawner->SpawnGrid(FVector::Zero(),10,10);
	GameStateRef->GameGrid = grid;

	if (Spawner)
	{
		HeroUnits = Spawner->SpawnUnitsFromArray(Spawner->HeroUnits, grid->GetHeroSpawnTiles());
		EnemyUnits = Spawner->SpawnUnitsFromArray(Spawner->EnemyUnits, grid->GetEnemySpawnTiles());
		for (ACUnit* EnemyUnit : EnemyUnits)
		{
			EnemyUnit->ControllingPlayerIndex = 0;
		}
	}
	AllUnits.Append(HeroUnits);
	AllUnits.Append(EnemyUnits);
	
	InitializeTurnOrder(AllUnits);
	ApplyPlayerCount(AllUnits);
}

void ACGameMode::RegisterAndExecuteConsequence(UCConsequence* inConsequence)
{
	if (!CommandList.IsEmpty())
	{
		CommandList.Last()->StoreConsequence(inConsequence);
	}
	else if (!CommandHistory.IsEmpty())
	{
		//This might happen if a consequence is triggered at the start of a turn when no command has been 
		//executed yet. If so, we store it in the previous turn's last command to make sure its registered 
		//somewhere, at least.
		CommandHistory.Last()->StoreConsequence(inConsequence);
	}
	inConsequence->ExecuteConsequence();
}

bool ACGameMode::TryAbilityUse(AController* inController, ACUnit* inUnit, FGameplayTag inItemSlotTag, ACGridTile* inTargetTile)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return false;
		}
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no units in the turn order"));
		return false;
	}

	if (GameStateRef->TurnOrder[0] != inUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is not first in the turn order"));
		return false;
	}

	if (!inUnit->IsControlledBy(inController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is not owned by controller"));
		return false;
	}

	/*UCItem* Item = inUnit->GetItemInSlot(inSlot);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("No item in this item slot"));
		return false;
	}
	if (!Item->IsValidTargetTile(inUnit, inTargetTile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target tile is not valid for the item"));
		return false;
	}
	UCCommand* NewCommand = Item->GenerateAbilityCommand(inController, inUnit, inTargetTile);
	if (!NewCommand)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability command on this item"));
		return false;
	}
	CommandList.Add(NewCommand);
	NewCommand->ExecuteCommand(inController);
	*/
	UCActionComponent* ActionComponent = inUnit->GetActionComp();
	FAbility OutAbility;
	ActionComponent->TryGetAbility(inItemSlotTag, OutAbility);


	if (!OutAbility.IsValidTargetTile(inUnit->GetTile(), inTargetTile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target tile is not valid for the item"));
		return false;
	}
	
	for (auto Action : OutAbility.Actions)
	{
		UCAction* NewAction = NewObject<UCAction>(ActionComponent, Action);
		NewAction->Initialize(ActionComponent);
		UCTargetableAction* Targetable = Cast<UCTargetableAction>(NewAction);
		if (Targetable)
		{
			Targetable->TargetTile = inTargetTile;
		}
		ActionList.Add(NewAction);
		NewAction->StartAction(inUnit);
	}

	//FString Log = FString("Executed command: ") + NewCommand->ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);
	//UCLogManager::Log(ELogCategory::LC_Gameplay,FString("Executed command: ") + NewCommand->ToString());
	return true;
}

bool ACGameMode::TryUndo(AController* inController)
{
	if (CommandList.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No commands in history"));
		return false;
	}

	UCCommand* LastCommand = CommandList.Last();
	if (LastCommand->GetCommandCreator() != inController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Command was not performed by this controller"));
		return false;
	}

	LastCommand->UndoAllConsequences();
	LastCommand->UndoCommand();

	CommandList.RemoveAtSwap(CommandList.Num() - 1);

	/*FString Log = FString("Undid command: ") + LastCommand->ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);*/
	UCLogManager::Log(ELogCategory::LC_Gameplay,FString("Undid command: ") + LastCommand->ToString());
	return true;
}

bool ACGameMode::TryEndTurn(AController* inController)
{
	if (!GameStateRef)
	{
		GameStateRef = GetGameState<ACGameState>();
		if (!GameStateRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Game State class"));
			return false;
		}
	}

	if (GameStateRef->TurnOrder.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no units in the turn order"));
		return false;
	}

	ACUnit* CurrentUnit = GameStateRef->TurnOrder[0];
	if (!CurrentUnit->IsControlledBy(inController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller does not manage current unit"));
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
	for (UCCommand* Command : CommandList)
	{
		CommandHistory.Add(Command);
	}
	CommandList.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Turn ended"));
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
