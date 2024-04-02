// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameMenuWidget::SetUp()
{
	Super::SetUp();
	QuitButton->OnPressed.AddDynamic(this,&UGameMenuWidget::QuitGame);
}

void UGameMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit, false);
}
