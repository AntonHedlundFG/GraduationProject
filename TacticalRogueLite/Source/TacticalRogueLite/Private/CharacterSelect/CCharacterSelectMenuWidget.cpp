

#include "CharacterSelect/CCharacterSelectMenuWidget.h"


void UCCharacterSelectMenuWidget::UpdateControllingPlayers(TArray<int> inArray)
{
	if (inArray.Num() != ControllingPlayerIndex.Num())
		return;

	for (int i = 0; i < ControllingPlayerIndex.Num(); i ++)
	{
		ControllingPlayerIndex[i] = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateCharacters(TArray<int> inArray)
{
	if (inArray.Num() != CharacterIndexes.Num())
		return;

	for (int i = 0; i < CharacterIndexes.Num(); i ++)
	{
		CharacterIndexes[i] = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateLocks(TArray<bool> inArray)
{
	if (inArray.Num() != LockedInfo.Num())
		return;

	for (int i = 0; i < LockedInfo.Num(); i ++)
	{
		LockedInfo[i] = inArray[i];
	}
}

void UCCharacterSelectMenuWidget::UpdateReadyStatus(TArray<bool> inArray)
{
	if (inArray.Num() != ReadyInfo.Num())
		return;

	for (int i = 0; i < ReadyInfo.Num(); i ++)
	{
		ReadyInfo[i] = inArray[i];
	}
}
