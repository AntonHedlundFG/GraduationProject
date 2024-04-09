

#include "CharacterSelect/CSetControllingPlayerWidget.h"

#include "CharacterSelect/CCharacterSelectController.h"
#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CCharacterSelectorWidget.h"



void UCSetControllingPlayerWidget::IncreaseIndex()
{
	if (!GameState)
		GameState = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());
	
	if (!GameState || GameState->LockedInfo[WidgetIndex])
		return;
	
	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());

	if (Controller)
	{
		int Max = GameState->PlayerCount;
		int PlayerIndex = GameState->ControllingPlayerIndex[WidgetIndex];
		PlayerIndex++;
		if (PlayerIndex > Max)
			PlayerIndex = 1;

		Controller->Server_UpdatePlayerIndex(WidgetIndex, PlayerIndex);
	}
	
}

void UCSetControllingPlayerWidget::DecreaseIndex()
{
	if (!GameState)
		GameState = Cast<ACCharacterSelectGameState>(GetWorld()->GetGameState());
	
	if (!GameState || GameState->LockedInfo[WidgetIndex])
		return;
	
	if (!Controller)
		Controller = Cast<ACCharacterSelectController>(GetWorld()->GetFirstPlayerController());

	if (Controller)
	{
		int Max = GameState->PlayerCount;
		int PlayerIndex = GameState->ControllingPlayerIndex[WidgetIndex];
		PlayerIndex--;
		if (PlayerIndex < 1 )
			PlayerIndex = Max;

		Controller->Server_UpdatePlayerIndex(WidgetIndex, PlayerIndex);
	}
}
