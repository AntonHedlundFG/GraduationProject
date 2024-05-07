

#include "CharacterSelect/CCharacterSelectController.h"

#include "CharacterSelect/CCharacterSelectGameState.h"

void ACCharacterSelectController::Server_UpdateSpriteIndex_Implementation(int InWidgetIndex, int InSpriteIndex)
{
	if (StateRef)
	{
		TArray<int> Sprites = StateRef->SpriteIndexes;
		//Den här, sprites är icke initialized
		Sprites[InWidgetIndex] = InSpriteIndex;
		StateRef->UpdateSprites(Sprites);
	}
}

void ACCharacterSelectController::BeginPlay()
{
	Super::BeginPlay();
	StateRef = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());
}

void ACCharacterSelectController::Server_UpdateReady_Implementation(int InWidgetIndex, bool InReadyStatus)
{
	if (StateRef)
	{
		TArray<bool> ReadyArray = StateRef->ReadyInfo;
		ReadyArray[InWidgetIndex] = InReadyStatus;

		StateRef->UpdateReadyStatus(ReadyArray);
	}
}

void ACCharacterSelectController::Server_UpdatePlayerIndex_Implementation(int InWidgetIndex, int InPlayerIndex)
{
	if (StateRef)
	{
		TArray<int> Players = StateRef->ControllingPlayerIndex;
		Players[InWidgetIndex] = InPlayerIndex;

		StateRef->UpdateControllingPlayers(Players);
	}
}

void ACCharacterSelectController::Server_UpdateCharacterIndex_Implementation(int InWidgetIndex, int InCharacterIndex)
{
	if (StateRef)
	{
		TArray<int> Characters = StateRef->CharacterIndexes;
		Characters[InWidgetIndex] = InCharacterIndex;
		StateRef->UpdateCharacters(Characters);
	}
}

