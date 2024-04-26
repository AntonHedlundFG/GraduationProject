

#include "CharacterSelect/CCharacterSelectGameMode.h"

#include "CLevelURLAsset.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CStartCharacterData.h"
#include "Items/CNamesAndItemsList.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/CRandomComponent.h"

ACCharacterSelectGameMode::ACCharacterSelectGameMode()
{
	Random = CreateDefaultSubobject<UCRandomComponent>(TEXT("Random"));
}

void ACCharacterSelectGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterToSaveManager();

	UCSaveGameManager::Get()->LoadGame();
	
	//Create a random seed
	const int Seed = FMath::RandRange(0000,9999);
	Random->InitializeFromStart(Seed);
	
	StateRef = GetGameState<ACCharacterSelectGameState>();
	if(StateRef)
	{
		StateRef->SetPlayerCountAndLocks(PlayerCount);
		StateRef->OnReadyToStart.AddDynamic(this, &ACCharacterSelectGameMode::CreateSaveGameAndStart);
		StateRef->BP_SetupUI();
	}

	//Begin-play done !!
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
	
	LOG_INFO("Ready To Start");

	FLevelLoadSettings LevelLoadSettings;
	LevelLoadSettings.NumberOfPlayers = GetPlayerCount();

	UnregisterFromSaveManager();
	
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
		//Save Names and Items	
		SaveGame->NamesAndItems.Empty();
		for (auto Data : SpawnUnitData)
		{
			SaveGame->NamesAndItems.Add(Data);
		}
		
		//Save controlling player indexes
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
