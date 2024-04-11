

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
	
	PlayerCount = UGameplayStatics::GetIntOption(OptionsString, NUMBER_OF_PLAYERS, DefaultPlayerCount);

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
	
	TArray<int> Characters = StateRef->CharacterIndexes;
	ControllingPlayers = StateRef->ControllingPlayerIndex;

	SpawnUnitData.Empty();
	int CharacterNumber = 1;
	
	for (auto Index : Characters)
	{
		FCNamesAndItemsList Data = StartCharacters->StartCharacterList[Index];
		FString HeroName = "Hero_";
		HeroName += FString::FromInt(CharacterNumber);
		Data.Name = HeroName;
		CharacterNumber++;
		SpawnUnitData.Add(Data);
	}

	/*
	*/
	UCSaveGameManager* SaveManager = UCSaveGameManager::Get();
	RegisterToSaveManager();
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
	//
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
	
}
