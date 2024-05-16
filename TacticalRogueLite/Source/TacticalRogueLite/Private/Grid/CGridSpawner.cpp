
#include "Grid/CGridSpawner.h"

#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Grid/CGrid.h"
#include "Grid/CGridRoom.h"
#include "Grid/CGridTile.h"
#include "GridContent/CPickUp.h"
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

void ACGridSpawner::RegisterToSaveManager()
{
	UCSaveGameManager::Get()->RegisterSavable(ESaveGameType::ESGT_Game, this);
}

void ACGridSpawner::UnregisterFromSaveManager()
{
	UCSaveGameManager::Get()->UnRegisterSavable(ESaveGameType::ESGT_Game, this);
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


TArray<ACUnit*> ACGridSpawner::SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles, TArray<FString> inNames, FGameplayTag inTeamTag, FGameplayTagContainer inClassTag) //TODO: ClassContainer will be moved to character selection.
{
	TArray<ACUnit*> Units;
	for (int i = 0 ; i < inSpawnTiles.Num() ; i ++)
	{
		if (i >= inUnits.Num()) break;
		ACUnit* Unit = SpawnUnit(inUnits[i], inSpawnTiles[i], inTeamTag, inClassTag);
		if(i < inNames.Num())
			Unit->SetUnitName(inNames[i]);
			
		Unit->ControllingPlayerIndex = i + 1;
		Units.Add(Unit);
	}
	return Units;
}

ACUnit* ACGridSpawner::SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile, FGameplayTag inTeamTag, FGameplayTagContainer inClassTag) //TODO: ClassContainer will be moved to character selection.
{
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActor<ACUnit>(inUnitType, SpawnPosition , FRotator::ZeroRotator);
	
	//Give Team Tag
	Unit->SetTeam(inTeamTag); //This is one single tag on the unit itself for easier checking in targetable tiles etc.
	Unit->GetActionComp()->ActiveGameplayTags.AppendTag(inTeamTag);
	Unit->GetActionComp()->ActiveGameplayTags.AppendTags(inClassTag); //TODO: ClassContainer will be moved to character selection.
	
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);

	return Unit;
}

ACUnit* ACGridSpawner::SpawnAndInitializeUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile,
	FCUnitSpawnDetails SpawnDetails, FGameplayTag inTeamTag) 
{
	FTransform SpawnTransform = FTransform();
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
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
	Unit->SetTeam(inTeamTag); //This is one single tag on the unit itself for easier checking in targetable tiles etc.
	Unit->GetActionComp()->ActiveGameplayTags.AppendTag(inTeamTag);
	Unit->GetActionComp()->ActiveGameplayTags.AppendTag(SpawnDetails.ClassTag);

	
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);
	
	return Unit;
}

ACPickUp* ACGridSpawner::SpawnPickUp(TSubclassOf<ACPickUp> inPickUpType, ACGridTile* inSpawnTile,
	FGameplayTagContainer inTags)
{
	const FVector SpawnPosition = inSpawnTile->GetActorLocation();
	TObjectPtr<ACPickUp> PickUp = GetWorld()->SpawnActor<ACPickUp>(inPickUpType, SpawnPosition , FRotator::ZeroRotator);
	
	inSpawnTile->SetContent(PickUp);
	PickUp->SetTile(inSpawnTile);

	return PickUp;
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

	//Decide on a random objective for the room.
	EVictoryConditions RoomWinCon = EVictoryConditions::EVC_KillEnemies;
	if (GameStateRef && GameStateRef->Random)
	{
		uint8 WinConInt = GameStateRef->Random->GetRandRange(1, static_cast<int32>(EVictoryConditions::EVC_MAX) - 1, false);
		RoomWinCon = static_cast<EVictoryConditions>(WinConInt);
	}

	//Spawn the room.
	if (bIsStartRoom)
	{
		NewRoom = inGrid->CreateStartRoom(inEnemyCount, RoomWinCon);
	}
	else
	{
		NewRoom = inGrid->CreateNewRoom(inEnemyCount, RoomWinCon);
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
		for (ACUnit* Enemy : Enemies)
		{
			//Insert each enemy randomly into the turn order, but not at the front of the queue.
			const int RandomTurnOrderIndex = FMath::RandRange(1, GameStateRef->GetCurrentTurnOrder(false).Num());
			GameStateRef->AddUnitToOrder(Enemy, RandomTurnOrderIndex - 1);
		}
	}
	
	//Add enemies to GameMode array.
	GameModeRef->AddEnemyUnits(Enemies);

	//Initialize Room WinCon and pass it to GameMode.
	TArray<ACPickUp*> Keys = TArray<ACPickUp*>();
	if (NewRoom->GetVictoryCondition() == EVictoryConditions::EVC_PickUpKeys)
	{
		
		for (auto Tile : NewRoom->GetKeySpawnTiles())
		{
			ACPickUp* Key = SpawnPickUp(KeyPickUp_BP, Tile);

			if (Key)
				Keys.Add(Key);
		}
	}
	if (!NewRoom->TryInitializeVictoryCondition(Enemies, Keys))
	{
		LOG_ERROR("Failed to Update Victory Condition for newly spawned Room");
	}
	
}

void ACGridSpawner::OnSave()
{
	USaveGame* SaveGameBase = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(ESaveGameType::ESGT_Game, SaveGameBase))
	{
		if (UCSaveGame* SaveGame = Cast<UCSaveGame>(SaveGameBase))
		{
			SaveGame->UnitDetails.Empty();
			for (auto Data : SpawnData)
			{
				SaveGame->UnitDetails.Add(Data);
			}
		
			SaveGame->ControllingPlayers.Empty();
			for (auto Data : ControllingPlayers)
			{
				SaveGame->ControllingPlayers.Add(Data);
			}
		}
	}
	else
	{
		LOG_ERROR("GridSpawner OnSave: Could not find Save Game Instance to save Unit Items and Data");
	}
}

void ACGridSpawner::OnLoad()
{
	USaveGame* SaveGameBase = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(ESaveGameType::ESGT_Game, SaveGameBase))
	{
		if (UCSaveGame* SaveGame = Cast<UCSaveGame>(SaveGameBase))
		{
			SpawnData.Empty();
			for (auto Data : SaveGame->UnitDetails)
			{
				SpawnData.Add(Data);
			}
		
			ControllingPlayers.Empty();
			for (auto Data : SaveGame->ControllingPlayers)
			{
				ControllingPlayers.Add(Data);
			}
		}
	}
	else
	{
		LOG_ERROR("GridSpawner OnLoad: Could not find Save Game Instance to load Unit Items and Data");
	}
}




