

#include "CharacterSelect/CCharacterSelectMenuWidget.h"

#include "CharacterSelect/CCharacterSelectGameState.h"
#include "CharacterSelect/CCharacterSelectorWidget.h"
#include "CharacterSelect/CSetControllingPlayerWidget.h"
#include "Components/CanvasPanel.h"


void UCCharacterSelectMenuWidget::UpdateAll()
{
	if (!StateRef)
		return;
	
	UpdateControllingPlayers(StateRef->ControllingPlayerIndex);
	UpdateCharacters(StateRef->CharacterIndexes);
	UpdateLocks(StateRef->LockedInfo);
	UpdateReadyStatus(StateRef->ReadyInfo);
}

void UCCharacterSelectMenuWidget::UpdateControllingPlayers(TArray<int> inArray)
{
	if (!StateRef)
		return;
	
	for (int i = 0; i < PlayerSettersArray.Num(); i ++)
	{
		if (UCSetControllingPlayerWidget* Widget = Cast<UCSetControllingPlayerWidget>(PlayerSettersArray[i]))
			Widget->UpdateControllingPlayerVisuals(inArray[i]);
	}
}

void UCCharacterSelectMenuWidget::UpdateCharacters(TArray<int> inArray)
{
	if (!StateRef)
		return;
	
	for (int i = 0; i < CharacterSelectorsArray.Num(); i ++)
	{
		if (UCCharacterSelectorWidget* Widget = Cast<UCCharacterSelectorWidget>(CharacterSelectorsArray[i]))
			Widget->UpdateCurrentCharacterVisuals(inArray[i],StateRef->SpriteIndexes[i]);
	}
}

void UCCharacterSelectMenuWidget::UpdateSprites(TArray<int> inArray)
{
	if (!StateRef)
		return;
	
	for (int i = 0; i < CharacterSelectorsArray.Num(); i ++)
	{
		if (UCCharacterSelectorWidget* Widget = Cast<UCCharacterSelectorWidget>(CharacterSelectorsArray[i]))
			Widget->UpdateCurrentCharacterVisuals(StateRef->CharacterIndexes[i],inArray[i]);
	}
}

void UCCharacterSelectMenuWidget::UpdateLocks(TArray<bool> inArray)
{
	if (!StateRef)
		return;
	
	for (int i = 0; i < PlayerSettersArray.Num(); i ++)
	{
		if (UCSetControllingPlayerWidget* Widget = Cast<UCSetControllingPlayerWidget>(PlayerSettersArray[i]))
			Widget->UpdateLockVisuals(inArray[i]);
	}
}

void UCCharacterSelectMenuWidget::UpdateReadyStatus(TArray<bool> inArray)
{
	if (!StateRef)
		return;
	
	for (int i = 0; i < CharacterSelectorsArray.Num(); i ++)
	{
		if (UCCharacterSelectorWidget* Widget = Cast<UCCharacterSelectorWidget>(CharacterSelectorsArray[i]))
			Widget->UpdateReadyVisuals(inArray[i]);
	}
}

void UCCharacterSelectMenuWidget::UpdatePlayerCount(int inCount)
{
}

void UCCharacterSelectMenuWidget::CreateMenuWidgets()
{
	for (int i = 0; i < 4; i ++)
	{
		//Create Controlling Player Widget
		UCSetControllingPlayerWidget* PlayerSetter = CreateWidget<UCSetControllingPlayerWidget>(this, PlayerSetterBP);
		PlayerSetter->WidgetIndex = i;
		PlayerSettersArray.Add(PlayerSetter);

		FAnchors PlayerAnchors;
		PlayerAnchors.Minimum = FVector2D(i / 4.0f, 0.3);
		PlayerAnchors.Maximum = FVector2D((i + 1) / 4.0f, 0.5);
		PlayerSetter->AddToViewport();
		PlayerSetter->SetAnchorsInViewport(PlayerAnchors);
		PlayerSetter->SetVisibility(ESlateVisibility::Visible);


		//Create Character Select Widget
		UCCharacterSelectorWidget* CharacterSelector = CreateWidget<UCCharacterSelectorWidget>(this, CharacterSelectorBP);
		CharacterSelector->WidgetIndex = i;
		CharacterSelectorsArray.Add(CharacterSelector);

		FAnchors CharacterAnchors;
		CharacterAnchors.Minimum = FVector2D(i / 4.0f, 0.5);
		CharacterAnchors.Maximum = FVector2D((i + 1) / 4.0f, 1);
		CharacterSelector->AddToViewport();
		CharacterSelector->SetAnchorsInViewport(CharacterAnchors);
		CharacterSelector->SetVisibility(ESlateVisibility::Visible);
	}

	UpdateAll();
}
