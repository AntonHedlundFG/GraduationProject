
#include "Grid/CGridSpawner.h"

#include "CGameMode.h"
#include "Grid/CGrid.h"
#include "Grid/CGridRoom.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Items/CInventoryComponent.h"
#include "Items/CNamesAndItemsList.h"
#include "Achievements/CVictoryCondition.h"
#include "GridContent/CAllEnemiesData.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

ACGridSpawner::ACGridSpawner()
{
	NamesAndItemList = TArray<FCNamesAndItemsList>();
	ControllingPlayers = TArray<int>();
}

void ACGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	RegisterToSaveManager();

	GameStateRef = Cast<ACGameState>(GetWorld()->GetGameState());
	GameModeRef = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
}

void ACGridSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromSaveManager();
	Super::EndPlay(EndPlayReason);
}


TArray<ACUnit*> ACGridSpawner::SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames)
{
	TArray<ACUnit*> Units;
	for (int i = 0 ; i < inSpawnTiles.Num() ; i ++)
	{
		if (i >= inUnits.Num()) break;
		ACUnit* Unit = SpawnUnit(inUnits[i], inSpawnTiles[i]);
		if(i < inNames.Num())
			Unit->SetUnitName(inNames[i]);
			
		Unit->ControllingPlayerIndex = i + 1;
		Units.Add(Unit);
	}
	return Units;
}

ACUnit* ACGridSpawner::SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile)
{
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	SpawnPosition.Z += 100;
	TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActor<ACUnit>(inUnitType, SpawnPosition , FRotator::ZeroRotator);
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);

	return Unit;
}

ACUnit* ACGridSpawner::SpawnAndInitializeUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile,
	TArray<UCItemData*> inEquipment, FString inName)
{
	ACUnit* Unit = SpawnUnit(inUnitType, inSpawnTile);
	UCInventoryComponent* Inventory = Unit->GetInventoryComp();

	//Try name unit
	if (!inName.IsEmpty())
		Unit->SetUnitName(inName);

	//Add items
	for (UCItemData* Item : inEquipment)
	{
		if (!Item)
			continue;
		
		if (Inventory->CheckValidEquipmentTag(Item->ItemSlot))
		{
			Inventory->TryEquipItem(Item);
		}
		else
		{
			Inventory->AddItem(Item);
		}
	}

	return Unit;
}

ACGrid* ACGridSpawner::SpawnGrid(FVector inGridCenter) const
{
	ACGrid* SpawnedGrid = GetWorld()->SpawnActor<ACGrid>(Grid, inGridCenter, FRotator::ZeroRotator);
	
	return SpawnedGrid;
}

void ACGridSpawner::SpawnRoomWithEnemies(ACGrid* inGrid, int inRoomLevel, int inEnemyCount, bool bIsStartRoom)
{
	//Create start room or regular room
	ACGridRoom* NewRoom;
	if (bIsStartRoom)
	{
		NewRoom = inGrid->CreateStartRoom(inEnemyCount);
	}
	else
	{
		NewRoom = inGrid->CreateNewRoom(inEnemyCount);
	}

	TArray<TSubclassOf<ACUnit>> EnemyTypes;
	for (auto Element : AllEnemyData->EnemyLevelAndType)
	{
		if (Element.Key <= inRoomLevel)
		{
			EnemyTypes.Add(Element.Value);
		}
	}
	TArray<TSubclassOf<ACUnit>> EnemiesToSpawn;
	if (GameStateRef)
	{
		UCRandomComponent* Random = GameStateRef->Random;
		
		for(int i = 0; i < NewRoom->GetEnemyCount(); i++)
		{
			const int index = Random->GetRandRange(0, EnemyTypes.Num() - 1);
			EnemiesToSpawn.Add(EnemyTypes[index]);
		}
	}

	//Spawn enemies and try to update victory condition
	const TArray<ACUnit*> Enemies = SpawnUnitsFromArray(EnemiesToSpawn, NewRoom->GetEnemySpawnTiles(), EnemyNames);
	if (GameModeRef)
	{
		GameModeRef->AddEnemyUnits(Enemies);
		if (!NewRoom->TryInitializeVictoryCondition(Enemies))
		{
			LOG_ERROR("Failed to Update Victory Condition for new Room");
		}
	}
}

void ACGridSpawner::OnSave()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		SaveGame->NamesAndItems.Empty();
		for (auto Data : NamesAndItemList)
		{
			SaveGame->NamesAndItems.Add(Data);
		}
		
		SaveGame->ControllingPlayers.Empty();
		for (auto Data : ControllingPlayers)
		{
			SaveGame->ControllingPlayers.Add(Data);
		}
	}
	else
	{
		LOG_ERROR("GridSpawner OnSave: Could not find Save Game Instance to save Unit Items and Data");
	}
}

void ACGridSpawner::OnLoad()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		NamesAndItemList.Empty();
		for (auto Data : SaveGame->NamesAndItems)
		{
			NamesAndItemList.Add(Data);
		}
		
		ControllingPlayers.Empty();
		for (auto Data : SaveGame->ControllingPlayers)
		{
			ControllingPlayers.Add(Data);
		}
	}
	else
	{
		LOG_ERROR("GridSpawner OnLoad: Could not find Save Game Instance to load Unit Items and Data");
	}
}




