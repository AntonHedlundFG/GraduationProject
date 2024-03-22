#include "Utility/SaveGame/CSaveGameManager.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/SaveGame/CSavable.h"


UCSaveGameManager* UCSaveGameManager::Instance = nullptr;

UCSaveGameManager* UCSaveGameManager::GetInstance()
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
}

UCSaveGame* UCSaveGameManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, UserIndex)) // Load Save Game if it exists
	{
		SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, UserIndex));
	}
	else // Create a new Save Game Instance if it doesn't exist
	{
		SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
	}

	TriggerLoadEvent(); // Trigger Load Event on all Savable Objects
	return SaveGameInstance;
}

UCSaveGame* UCSaveGameManager::GetSaveGameInstance()
{
	if(SaveGameInstance == nullptr) // Load Save Game if we have no active Save Game Instance
	{
		return LoadGame();
	}

	return SaveGameInstance; // Return the active Save Game Instance
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

