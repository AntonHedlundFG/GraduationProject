

#include "CharacterSelect/CCharacterSelectorWidget.h"

#include "CharacterSelect/CCharacterSelectController.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CStartCharacterData.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"


void UCCharacterSelectorWidget::IncreaseIndex()
{
	if (!CanInteract() || GameState->ReadyInfo[WidgetIndex])
		return;

	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());
	
	if (Controller && GameState)
	{
		int CharacterIndex = GameState->CharacterIndexes[WidgetIndex];
		CharacterIndex++;
		if (CharacterIndex >= StartCharacters->StartCharacterList.Num())
			CharacterIndex = 0;

		Controller->Server_UpdateSpriteIndex(WidgetIndex,0);
		Controller->Server_UpdateCharacterIndex(WidgetIndex, CharacterIndex);
	}
}

void UCCharacterSelectorWidget::DecreaseIndex()
{
	if (!CanInteract() || GameState->ReadyInfo[WidgetIndex])
		return;
	
	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());
	
	if (Controller && GameState)
	{
		int CharacterIndex = GameState->CharacterIndexes[WidgetIndex];
		CharacterIndex--;
		if (CharacterIndex < 0 )
			CharacterIndex = StartCharacters->StartCharacterList.Num() - 1;

		Controller->Server_UpdateSpriteIndex(WidgetIndex,0);
		Controller->Server_UpdateCharacterIndex(WidgetIndex, CharacterIndex);
	}
}


void UCCharacterSelectorWidget::ToggleReady()
{
	if (!CanInteract())
		return;

	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());
	
	if (Controller && GameState)
	{
		const bool bIsReady = GameState->ReadyInfo[WidgetIndex];
		Controller->Server_UpdateReady(WidgetIndex, !bIsReady);
	}
}

bool UCCharacterSelectorWidget::CanInteract()
{
	if (!GameState)
		GameState = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());

	if (!GameState || WidgetIndex > GameState->CharacterIndexes.Num())
		return false;

	if(const AOnlinePlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AOnlinePlayerState>())
	{
		return PlayerState->PlayerIndex == GameState->ControllingPlayerIndex[WidgetIndex];
	}

	return false;
}

void UCCharacterSelectorWidget::StepSpriteIndex(int i)
{
	if (!CanInteract())
		return;

	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());
	if (!GameState)
		GameState = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());
	
	if (Controller && GameState)
	{
		int CharacterIndex = GameState->CharacterIndexes[WidgetIndex];
		int Direction = (i >= 0)? 1: -1;
		int Translation = GameState->SpriteIndexes[WidgetIndex] + Direction;
		CurrentSelectedSpriteIndex = (Translation <0)? StartCharacters->GetSpriteListLength(CharacterIndex)-1:(Translation >= StartCharacters->GetSpriteListLength(CharacterIndex))? 0:Translation;
		Controller->Server_UpdateSpriteIndex(WidgetIndex,CurrentSelectedSpriteIndex);
	}
}
