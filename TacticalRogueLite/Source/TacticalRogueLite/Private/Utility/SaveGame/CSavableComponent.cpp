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
