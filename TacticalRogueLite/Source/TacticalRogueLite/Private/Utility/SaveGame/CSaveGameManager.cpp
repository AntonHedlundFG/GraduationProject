#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"
#include "Utility/SaveGame/CSaveGame_Settings.h"

UCSaveGameManager* UCSaveGameManager::Instance = nullptr;

UCSaveGameManager* UCSaveGameManager::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCSaveGameManager>();
		Instance->AddToRoot(); // Prevent Garbage Collection
		Instance->SaveSlots.Add(ESaveGameType::ESGT_Game, "GameSaveSlot");
		Instance->SaveSlots.Add(ESaveGameType::ESGT_Settings, "SettingsSaveSlot");
	}
	return Instance;
}

void UCSaveGameManager::RegisterSavable(ESaveGameType SaveGameType, ICSavable* Savable)
{
	if (!Savables.Contains(SaveGameType))
	{
		Savables.Add(SaveGameType, TArray<ICSavable*>());
	}
	Savables[SaveGameType].AddUnique(Savable);
}

void UCSaveGameManager::UnRegisterSavable(ESaveGameType SaveGameType, ICSavable* Savable)
{
	if (Savables.Contains(SaveGameType))
	{
		Savables[SaveGameType].RemoveSingle(Savable);
	}
}

USaveGame* UCSaveGameManager::CreateNewSave(ESaveGameType SaveGameType)
{
	USaveGame* NewSaveGame = nullptr;
	switch (SaveGameType)
	{
	case ESaveGameType::ESGT_Game:
		NewSaveGame = Cast<USaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		break;
	case ESaveGameType::ESGT_Settings:
		NewSaveGame = Cast<USaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame_Settings::StaticClass()));
		break;
	default:
		break;
	}

	if (NewSaveGame)
	{
		SaveGameInstances.Remove(SaveGameType);
		SaveGameInstances.Add(SaveGameType, NewSaveGame);
		LOG_INFO("New SaveGame Instance Created");
	}
	return NewSaveGame;
}

void UCSaveGameManager::SaveGame(ESaveGameType SaveGameType)
{
	if (!SaveGameInstances.Contains(SaveGameType))
	{
		CreateNewSave(SaveGameType);
	}

	USaveGame* SaveGameInstance = SaveGameInstances[SaveGameType];
	if (SaveGameInstance)
	{
		TriggerSaveEvent(SaveGameType);
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlots[SaveGameType], UserIndex);
		LOG_INFO("SaveGame Saved");
	}
}

void UCSaveGameManager::LoadGame(ESaveGameType SaveGameType)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlots[SaveGameType], UserIndex))
	{
		USaveGame* LoadedGame = Cast<USaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlots[SaveGameType], UserIndex));
		if (LoadedGame)
		{
			SaveGameInstances.Add(SaveGameType, LoadedGame);
			LOG_INFO("SaveGame Loaded");
			TriggerLoadEvent(SaveGameType);
		}
	}
	else
	{
		LOG_INFO("No SaveGame Found When Loading");
	}
}

bool UCSaveGameManager::TryGetSaveGame(ESaveGameType SaveGameType, USaveGame*& inSaveGame)
{
	if (SaveGameInstances.Contains(SaveGameType) && SaveGameInstances[SaveGameType])
	{
		inSaveGame = SaveGameInstances[SaveGameType];
		return true;
	}
	return false;
}

void UCSaveGameManager::TriggerSaveEvent(ESaveGameType SaveGameType)
{
	if (Savables.Contains(SaveGameType))
	{
		for (ICSavable* Savable : Savables[SaveGameType])
		{
			Savable->OnSave();
		}
	}
}

void UCSaveGameManager::TriggerLoadEvent(ESaveGameType SaveGameType)
{
	if (Savables.Contains(SaveGameType))
	{
		for (ICSavable* Savable : Savables[SaveGameType])
		{
			Savable->OnLoad();
		}
	}
}

