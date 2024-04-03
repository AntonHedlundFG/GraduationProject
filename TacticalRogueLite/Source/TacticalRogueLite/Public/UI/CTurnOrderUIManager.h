// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "UObject/NoExportTypes.h"
#include "CTurnOrderUIManager.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API ACTurnOrderUIManager : public AActor
{
	
	TQueue<UCTurnOrderPortraitWidget*> WidgetPool;
	UPROPERTY()
	ACGameState* GameState;
	GENERATED_BODY()
	virtual void BeginPlay() override;
	void UpdateTurnList();

	//PoolingSystem will be refactored later on
	UCTurnOrderPortraitWidget* CreatePortraitWidget();
	UCTurnOrderPortraitWidget* DeQueuePortraitWidget();
	void EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;
};
