// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GridContent/CUnit.h"
#include "CEndTurnButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCEndTurnButtonUI : public UUserWidget
{
	GENERATED_BODY()

	bool bIsOpen = false;
	UPROPERTY()
	ACUnit* CurrentUnit;
	int ItemCharges;
	int LastUpdateItemCharges;
	UFUNCTION()
	void EndTurn();
	UFUNCTION()
	void OnChargesChanged();
	UFUNCTION()
	void OnTurnUpdated();
	void ShowEndButton();
	void HideEndButton();
	UPROPERTY()
	ACGameState* GameState;
	UPROPERTY(meta = (BindWidget))
	UButton* EndButton;
	UPROPERTY( Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* FadeInAnimation;
	void UpdateItemCharges();
	virtual void NativeConstruct() override;
};
