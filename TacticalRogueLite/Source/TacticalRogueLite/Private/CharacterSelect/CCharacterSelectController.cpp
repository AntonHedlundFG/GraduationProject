

#include "CharacterSelect/CCharacterSelectController.h"

#include "CharacterSelect/CCharacterSelectGameState.h"

void ACCharacterSelectController::BeginPlay()
{
	Super::BeginPlay();
	StateRef = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());
}

void ACCharacterSelectController::Server_UpdateReady(int InWidgetIndex, bool InReadyStatus)
{
	if (StateRef)
	{
		TArray<bool> ReadyArray = StateRef->ReadyInfo;
		ReadyArray[InWidgetIndex] = InReadyStatus;

		StateRef->OnRep_UpdateReadyStatus(ReadyArray);
	}
}

void ACCharacterSelectController::Server_UpdatePlayerIndex(int InWidgetIndex, int InPlayerIndex)
{
	if (StateRef)
	{
		TArray<int> Players = StateRef->ControllingPlayerIndex;
		Players[InWidgetIndex] = InPlayerIndex;

		StateRef->OnRep_UpdateControllingPlayers(Players);
	}
}

void ACCharacterSelectController::Server_UpdateCharacterIndex(int InWidgetIndex, int InCharacterIndex)
{
	if (StateRef)
	{
		TArray<int> Characters = StateRef->CharacterIndexes;
		Characters[InWidgetIndex] = InCharacterIndex;

		StateRef->OnRep_UpdateCharacters(Characters);
	}
}

