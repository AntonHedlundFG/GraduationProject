// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CResizableWindow.h"
#include "GameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UGameMenuWidget : public UCMovableWindow
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	virtual void SetUp() override;
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void GoToMainMenu();
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainMenuRef;
};
