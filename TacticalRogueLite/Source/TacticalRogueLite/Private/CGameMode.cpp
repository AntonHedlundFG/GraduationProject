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

void ACGameMode::BeginPlay()
{
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
	GameGrid = Spawner->SpawnGrid(FVector::Zero(),10,10);
	
	//This can probably be done better/cleaner
	Spawner->SpawnUnitsFromArray(Spawner->HeroUnits, GameGrid->GetHeroSpawnTiles());
	Spawner->SpawnUnitsFromArray(Spawner->EnemyUnits, GameGrid->GetEnemySpawnTiles());

	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACUnit::StaticClass(), OutActors);
	TArray<ACUnit*> AllUnits;
	for (AActor* Actor : OutActors)
	{
		ACUnit* Unit = Cast<ACUnit>(Actor);
		if (Unit)
			AllUnits.Add(Unit);
	}
	
	//We might want to move this to another place later, 
	// as all Units might not be spawned yet.
	InitializeTurnOrder(AllUnits);
	ApplyPlayerCount(AllUnits);
}

bool ACGameMode::TryAbilityUse(AController* inController, ACUnit* inUnit, const EItemSlots inSlot, ACGridTile* inTargetTile)
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

	UCItem* Item = inUnit->GetItemInSlot(inSlot);
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


	NewCommand->ExecuteCommand(inController);
	CommandList.Add(NewCommand);

	FString Log = FString("Executed command: ") + NewCommand->ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);

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

	LastCommand->UndoCommand();

	CommandList.RemoveAtSwap(CommandList.Num() - 1);

	FString Log = FString("Undid command: ") + LastCommand->ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);
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
