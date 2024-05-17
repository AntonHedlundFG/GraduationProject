#include "Utility/SaveGame/CSavableComponent.h"

void UCSavableComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterToSaveManager();
}

void UCSavableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnregisterFromSaveManager();
}

void UCSavableComponent::RegisterToSaveManager()
{
	UCSaveGameManager::Get()->RegisterSavable(ESaveGameType::ESGT_Game, this);
}

void UCSavableComponent::UnregisterFromSaveManager()
{
	UCSaveGameManager::Get()->UnRegisterSavable(ESaveGameType::ESGT_Game, this);
}
