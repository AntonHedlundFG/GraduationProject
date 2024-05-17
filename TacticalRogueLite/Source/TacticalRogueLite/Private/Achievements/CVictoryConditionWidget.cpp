

#include "Achievements/CVictoryConditionWidget.h"

#include "CGameMode.h"
#include "CGameState.h"
#include "CLevelURLAsset.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

void UCVictoryConditionWidget::SetUp()
{
	// RestartButton->OnPressed.AddUniqueDynamic(this,&UCVictoryConditionWidget::Restart);
	MainMenuButton->OnPressed.AddUniqueDynamic(this, &UCVictoryConditionWidget::GoToMainMenu);
	QuitButton->OnPressed.AddUniqueDynamic(this,&UCVictoryConditionWidget::QuitGame);
	bIsOpen = false;
	SetVisibility(ESlateVisibility::Hidden);

	if (ACGameState* StateRef = Cast<ACGameState>(GetWorld()->GetGameState()))
	{
		StateRef->OnGameIsOver.AddDynamic(this, &UCVictoryConditionWidget::OpenWindow);
	}
}

void UCVictoryConditionWidget::OpenWindow()
{
	SetVisibility(ESlateVisibility::Visible);
	
	if (!GetWorld()->GetAuthGameMode())
	{
		// RestartButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCVictoryConditionWidget::Restart()
{
	if (!LevelURLAsset)
		return;
	
	if (ACGameMode* GameModeRef = Cast<ACGameMode>(GetWorld()->GetAuthGameMode()))
	{
		FLevelLoadSettings LevelLoadSettings;
		LevelLoadSettings.NumberOfPlayers = GameModeRef->GetNumPlayers();
		const FString URLString = LevelURLAsset->URLOfLevelByName("CharacterSelectLevel", LevelLoadSettings);

		UCSaveGameManager* SaveManager = UCSaveGameManager::Get();
		if (UCSaveGame* NewSaveGame = Cast<UCSaveGame>(SaveManager->CreateNewSave(ESaveGameType::ESGT_Game)))
		{
			NewSaveGame->PlayerCount = GameModeRef->GetNumPlayers();
			SaveManager->SaveGame(ESaveGameType::ESGT_Game);
		}
		else
		{
			return;
		}

		if (UEpicOnlineSubsystem* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>())
		{
			OnlineSystem->ServerTravel(URLString);
		}
	}
}

void UCVictoryConditionWidget::GoToMainMenu()
{
	if (auto* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>())
		OnlineSystem->DestroySession();

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		GameMode->ReturnToMainMenuHost();
	}
	else if (MainMenuRef)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuRef);
	}
}

void UCVictoryConditionWidget::QuitGame()
{
	if (auto* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>())
		OnlineSystem->DestroySession();

	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit, false);
}

