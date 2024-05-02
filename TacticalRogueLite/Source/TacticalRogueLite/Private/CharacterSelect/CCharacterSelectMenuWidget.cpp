

#include "CharacterSelect/CCharacterSelectMenuWidget.h"

#include "CharacterSelect/CCharacterSelectorWidget.h"
#include "CharacterSelect/CSetControllingPlayerWidget.h"


void UCCharacterSelectMenuWidget::UpdateControllingPlayers(TArray<int> inArray)
{
	for (int i = 0; i < PlayerSettersArray.Num(); i ++)
	{
		PlayerSettersArray[i]->ControllingPlayerIndex = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateCharacters(TArray<int> inArray)
{
	for (int i = 0; i < CharacterSelectorsArray.Num(); i ++)
	{
		CharacterSelectorsArray[i]->CharacterTypeIndex = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateLocks(TArray<bool> inArray)
{
	for (int i = 0; i < PlayerSettersArray.Num(); i ++)
	{
		PlayerSettersArray[i]->bIsLocked = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateReadyStatus(TArray<bool> inArray)
{
	for (int i = 0; i < CharacterSelectorsArray.Num(); i ++)
	{
		CharacterSelectorsArray[i]->bIsReadyToStart = inArray[i];
	}
}
