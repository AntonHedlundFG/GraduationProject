#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"

UCSaveGameManager* UCSaveGameManager::Instance = nullptr;

UCSaveGameManager* UCSaveGameManager::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCSaveGameManager>();
		Instance->AddToRoot(); // Prevent Garbage Collection
	}
	return Instance;
}

void UCSaveGameManager::SaveGame()
{
	if(SaveGameInstance == nullptr) // Create a new Save Game Instance if it doesn't exist
		SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));

	TriggerSaveEvent(); // Trigger Save Event on all Savable Objects
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlot, UserIndex); // Save Game
	LOG_INFO("SaveGame Saved");
}

void UCSaveGameManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, UserIndex)) // Load Save Game if it exists
	{
		SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, UserIndex));
		LOG_INFO("SaveGame Loaded");
	}
	else
	{
		LOG_WARNING("No SaveGame Found When Loading");
		
	}
	TriggerLoadEvent(); // Trigger Load Event on all Savable Objects
}


bool UCSaveGameManager::TryGetSaveGame(UCSaveGame*& inSaveGame)
{
	if(SaveGameInstance == nullptr)
	{
		return false;
	}
	inSaveGame = SaveGameInstance;
	return true; // Return the active Save Game Instance
}

void UCSaveGameManager::TriggerSaveEvent()
{
	for (ICSavable* Savable : Savables)
	{
		Savable->OnSave();
	}
}

void UCSaveGameManager::TriggerLoadEvent()
{
	for (ICSavable* Savable : Savables)
	{
		Savable->OnLoad();
	}
}

