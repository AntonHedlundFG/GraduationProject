
#include "Grid/CGridSpawner.h"

#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Grid/CGrid.h"
#include "Grid/CGridRoom.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Items/CInventoryComponent.h"
#include "GridContent/UnitDataAssets/CAllEnemiesData.h"
#include "GridContent/UnitDataAssets/CUnitSpawnDetails.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/Logging/CLogManager.h"

ACGridSpawner::ACGridSpawner()
{
	SpawnData = TArray<FCUnitSpawnDetails>();
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


TArray<ACUnit*> ACGridSpawner::SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames, FGameplayTag inTeamTag)
{
	TArray<ACUnit*> Units;
	for (int i = 0 ; i < inSpawnTiles.Num() ; i ++)
	{
		if (i >= inUnits.Num()) break;
		ACUnit* Unit = SpawnUnit(inUnits[i], inSpawnTiles[i], inTeamTag);
		if(i < inNames.Num())
			Unit->SetUnitName(inNames[i]);
			
		Unit->ControllingPlayerIndex = i + 1;
		Units.Add(Unit);
	}
	return Units;
}

ACUnit* ACGridSpawner::SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, FGameplayTag inTeamTag)
{
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	SpawnPosition.Z += 100;
	TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActor<ACUnit>(inUnitType, SpawnPosition , FRotator::ZeroRotator);
	
	//Give Team Tag
	Unit->GetActionComp()->ActiveGameplayTags.AppendTag(inTeamTag);
	
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);

	return Unit;
}

ACUnit* ACGridSpawner::SpawnAndInitializeUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile,
	FCUnitSpawnDetails SpawnDetails, FGameplayTag inTeamTag)
{
	FTransform SpawnTransform = FTransform();
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	SpawnPosition.Z += 100;
	SpawnTransform.SetLocation(SpawnPosition);

	const TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActorDeferred<ACUnit>(inUnitType, SpawnTransform, this);
	
	UCInventoryComponent* Inventory = Unit->GetInventoryComp();

	//Try name unit
	if (!SpawnDetails.Name.IsEmpty())
		Unit->SetUnitName(SpawnDetails.Name);

	//Try set sprite
	if (SpawnDetails.Sprite)
		Unit->SetSprite(SpawnDetails.Sprite);
	
	Unit->FinishSpawning(SpawnTransform);

	//Add items
	for (UCItemData* Item : SpawnDetails.Items)
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

	//Give Team Tag
	Unit->GetActionComp()->ActiveGameplayTags.AppendTag(inTeamTag);
	
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);
	
	return Unit;
}



ACGrid* ACGridSpawner::SpawnGrid(FVector inGridCenter) const
{
	ACGrid* SpawnedGrid = GetWorld()->SpawnActor<ACGrid>(Grid, inGridCenter, FRotator::ZeroRotator);
	
	return SpawnedGrid;
}

void ACGridSpawner::SpawnRoomWithEnemies(ACGrid* inGrid, int inRoomLevel, int inEnemyCount, bool bIsStartRoom)
{
	//Create start room or regular room, update camera bounds
	ACGridRoom* NewRoom;
	if (bIsStartRoom)
	{
		NewRoom = inGrid->CreateStartRoom(inEnemyCount);
	}
	else
	{
		NewRoom = inGrid->CreateNewRoom(inEnemyCount);
	}
	const int EnemyAmount = NewRoom->GetEnemyCount();

	//Check that we have enemy data to use for spawning
	if (!AllEnemyData)
	{
		LOG_ERROR("GridSpawner SpawnRoomWithEnemies: Missing Enemy Data Asset, Can't Spawn Enemies");
		return;
	}

	//Get valid enemies to spawn based on room level
	TArray<FCEnemyType> PossibleEnemyTypes;
	for (auto Element : AllEnemyData->EnemyLevelAndType)
	{
		if (Element.Level <= inRoomLevel)
		{
			PossibleEnemyTypes.Add(Element);
		}
	}

	//If there are no valid enemies, spawn nothing
	if (PossibleEnemyTypes.Num() <= 0)
	{
		LOG_WARNING("GridSpawner SpawnRoomWithEnemies: Couldn't find viable enemies to spawn");
		return;
	}

	//If no GameState found; issue warning, spawn no enemies.
	if (!GameStateRef)
	{
		LOG_WARNING("GridSpawner SpawnRoomWithEnemies: Missing Game State reference, can't spawn new units");
		return;
	}
	//If no GameState found; issue warning, spawn no enemies.
	if (!GameModeRef)
	{
		LOG_WARNING("GridSpawner SpawnRoomWithEnemies: Missing Game Mode reference, can't spawn new units");
		return;
	}
	
	//Spawn enemies of random valid types; equip items, set name, and set sprite.
	TArray<ACUnit*> Enemies;
	UCRandomComponent* Random = GameStateRef->Random;
	for(int i = 0; i < EnemyAmount; i++)
	{
		const int index = Random->GetRandRange(0, PossibleEnemyTypes.Num() - 1, false);

		const FCUnitSpawnDetails EnemyDetails = PossibleEnemyTypes[index].CharacterDetails;

		ACUnit* Enemy = SpawnAndInitializeUnit(EnemyUnit_BP, NewRoom->GetEnemySpawnTiles()[i], EnemyDetails, FGameplayTag::RequestGameplayTag("Unit.IsEnemy"));
		// Enemy->OnRep_SetAppearance(EnemyDetails.Sprite);
		Enemies.Add(Enemy);
	}
	
	//If this is not the starting room, add the new enemies to the back of the turn order list.
	if (!bIsStartRoom)
	{
		GameStateRef->TurnOrder.Append(Enemies);
		GameStateRef->OnRep_TurnOrder();
	}
	
	//Add enemies to GameMode array.
	GameModeRef->AddEnemyUnits(Enemies);
	if (!NewRoom->TryInitializeVictoryCondition(Enemies))
	{
		LOG_ERROR("Failed to Update Victory Condition for newly spawned Room");
	}
	
}

void ACGridSpawner::OnSave()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		SaveGame->NamesAndItems.Empty();
		for (auto Data : SpawnData)
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
		SpawnData.Empty();
		for (auto Data : SaveGame->NamesAndItems)
		{
			SpawnData.Add(Data);
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




