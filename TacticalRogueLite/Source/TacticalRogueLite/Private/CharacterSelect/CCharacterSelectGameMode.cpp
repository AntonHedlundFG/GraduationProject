

#include "CharacterSelect/CCharacterSelectGameMode.h"

#include "CLevelURLAsset.h"
#include "OnlineSubsystemUtils.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CStartCharacterData.h"
#include "Items/CNamesAndItemsList.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

ACCharacterSelectGameMode::ACCharacterSelectGameMode()
{
}

void ACCharacterSelectGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterToSaveManager();

	UCSaveGameManager::Get()->LoadGame();

	
	StateRef = GetGameState<ACCharacterSelectGameState>();
	if(StateRef)
	{
		StateRef->PlayerCount = PlayerCount;
		StateRef->OnReadyToStart.AddDynamic(this, &ACCharacterSelectGameMode::CreateSaveGameAndStart);
	}
}

void ACCharacterSelectGameMode::CreateSaveGameAndStart()
{
	if (!StartCharacters)
	{
		LOG_WARNING("No Start Character Data in Game Mode");
		return;
	}
	if (!StateRef)
	{
		LOG_WARNING("No Game State Reference in Game Mode");
		return;
	}
	if (!LevelURLAsset)
	{
		LOG_WARNING("No Level URL Asset in Game Mode");
		return;
	}
	
	TArray<int> Characters = StateRef->CharacterIndexes;
	ControllingPlayers = StateRef->ControllingPlayerIndex;

	SpawnUnitData.Empty();
	int CharacterNumber = 1;
	
	for (auto Index : Characters)
	{
		FCNamesAndItemsList Data = StartCharacters->StartCharacterList[Index];

		FString HeroName = "Hero ";
		HeroName += FString::FromInt(CharacterNumber);
		HeroName += "_" + Data.Name;
		Data.Name = HeroName;
		CharacterNumber++;
		SpawnUnitData.Add(Data);
	}

	/*
	*/
	UCSaveGameManager* SaveManager = UCSaveGameManager::Get();
	SaveManager->SaveGame();
	UnregisterFromSaveManager();
	
	LOG_INFO("Ready To Start");

	FLevelLoadSettings LevelLoadSettings;
	LevelLoadSettings.NumberOfPlayers = GetPlayerCount();
	
	FString URLString = LevelURLAsset->URLOfLevelByName("GameLevel", LevelLoadSettings);
	
	if (UEpicOnlineSubsystem* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>())
	{
		OnlineSystem->ServerTravel(URLString);
	}
	
}

void ACCharacterSelectGameMode::OnSave()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		
		SaveGame->NamesAndItems.Empty();
		for (auto Data : SpawnUnitData)
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
		LOG_ERROR("Could not find Save Game Instance to save Unit Items and Data in Game Mode");
	}
}

void ACCharacterSelectGameMode::OnLoad()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		PlayerCount = SaveGame->PlayerCount;
	}
	else
	{
		LOG_ERROR("Couldn't find Save Game Instance to load Player Count in Character Select Menu");
	}
}
