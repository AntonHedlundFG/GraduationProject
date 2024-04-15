// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimatingTurnOrderBox.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "Blueprint/UserWidget.h"
#include "CTurnOrderUI.generated.h"
class ACUnit;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCTurnOrderUI : public UUserWidget
{
	GENERATED_BODY()
	TQueue<UCTurnOrderPortraitWidget*> WidgetPool;
	UPROPERTY()
	TMap<ACUnit*,UCTurnOrderPortraitWidget*> ActivePortraits;
	UPROPERTY()
	ACGameState* GameState;
	UCTurnOrderPortraitWidget* CreatePortraitWidget();
	UCTurnOrderPortraitWidget* DeQueuePortraitWidget();
	void HandleEnqueue(UCTurnOrderPortraitWidget* widget);
	void HandleDequeue(UCTurnOrderPortraitWidget* widget);
	UCTurnOrderPortraitWidget* GetActiveWidget(ACUnit* key);
	UFUNCTION()
	void UpdateTurnList();
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UAnimatingTurnOrderBox* TurnOrderBox;
public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;
	void EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget);
};
