

#include "CharacterSelect/CCharacterSelectGameMode.h"

#include "CLevelURLAsset.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CStartCharacterData.h"
#include "Items/CNamesAndItemsList.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

ACCharacterSelectGameMode::ACCharacterSelectGameMode()
{
	PlayerCount = UGameplayStatics::GetIntOption(OptionsString, NUMBER_OF_PLAYERS, DefaultPlayerCount);
}

void ACCharacterSelectGameMode::BeginPlay()
{
	Super::BeginPlay();

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
	
	SpawnUnitData.Empty();
	int CharacterNumber = 1;
	TArray<int> Characters = StateRef->CharacterIndexes;
	for (auto Index : Characters)
	{
		FCNamesAndItemsList Data;
		Data.Items = StartCharacters->StartCharacterList[Index].ItemList;
		Data.Name = "Hero_" + CharacterNumber;
		CharacterNumber++;
		SpawnUnitData.Add(Data);
	}

	/*
	UCSaveGameManager* SaveManager = UCSaveGameManager::Get();
	RegisterToSaveManager();
	SaveManager->SaveGame();
	*/
	LOG_INFO("Ready To Start");
}

void ACCharacterSelectGameMode::OnSave()
{
	UCSaveGame* SaveGame = nullptr;
	if (UCSaveGameManager::Get()->TryGetSaveGame(SaveGame))
	{
		SaveGame->NamesAndItems = SpawnUnitData;
	}
	else
	{
		LOG_ERROR("Could not find Save Game Instance to save Unit Items and Data in Game Mode");
	}
}

void ACCharacterSelectGameMode::OnLoad()
{
	
}
