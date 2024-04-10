// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameMenuWidget::SetUp()
{
	Super::SetUp();
	QuitButton->OnPressed.AddDynamic(this,&UGameMenuWidget::QuitGame);
	MainMenuButton->OnPressed.AddDynamic(this, &UGameMenuWidget::QuitGame);
}

void UGameMenuWidget::QuitGame()
{
	auto* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();
	if (OnlineSystem)
		OnlineSystem->DestroySession();

	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit, false);
}

void UGameMenuWidget::GoToMainMenu()
{
	auto* OnlineSystem = GetGameInstance()->GetSubsystem<UEpicOnlineSubsystem>();
	if (OnlineSystem)
		OnlineSystem->DestroySession();

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		GameMode->ReturnToMainMenuHost();
	}
	else if (MainMenuRef)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuRef);
	}
}
