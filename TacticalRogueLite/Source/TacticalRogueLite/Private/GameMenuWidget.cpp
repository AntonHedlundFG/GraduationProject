// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "TacticalRogueLite/OnlineSystem/Public/EpicOnlineSubsystem.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameMenuWidget::SetUp()
{
	Super::SetUp();
	QuitButton->OnPressed.AddUniqueDynamic(this,&UGameMenuWidget::QuitGame);
	MainMenuButton->OnPressed.AddUniqueDynamic(this, &UGameMenuWidget::GoToMainMenu);
	bIsOpen = false;
	MainCanvas->SetVisibility(ESlateVisibility::Collapsed);
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
		auto* PC = Cast<AOnlinePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC)
			PC->Server_ManualDisconnect();
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuRef);
	}
}
