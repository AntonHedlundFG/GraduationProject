
#include "Grid/CGridSpawner.h"

#include "Grid/CGrid.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Items/CInventoryComponent.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

ACGridSpawner::ACGridSpawner()
{
	
}

void ACGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	RegisterToSaveManager();
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

ACGrid* ACGridSpawner::SpawnGrid(FVector inGridCenter, int inRows, int inColumns)
{
	ACGrid* SpawnedGrid = GetWorld()->SpawnActor<ACGrid>(Grid, inGridCenter, FRotator::ZeroRotator);
	SpawnedGrid->GenerateTiles(inRows, inColumns);

	return SpawnedGrid;
}

void ACGridSpawner::OnSave()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		SaveGame->NamesAndItems = NamesAndItemList;
	}
	else
	{
		LOG_ERROR("Could not find Save Game Instance to save Unit Items and Data");
	}
}

void ACGridSpawner::OnLoad()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		NamesAndItemList = SaveGame->NamesAndItems;
	}
	else
	{
		LOG_ERROR("Could not find Save Game Instance to load Unit Items and Data");
	}
}




