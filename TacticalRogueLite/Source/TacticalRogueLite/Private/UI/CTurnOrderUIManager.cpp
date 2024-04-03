// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderUIManager.h"

void ACTurnOrderUIManager::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddUniqueDynamic(this,&ACTurnOrderUIManager::UpdateTurnList);
	}
}

void ACTurnOrderUIManager::UpdateTurnList()
{
	
}

UCTurnOrderPortraitWidget* ACTurnOrderUIManager::CreatePortraitWidget()
{
	
}
UCTurnOrderPortraitWidget* ACTurnOrderUIManager::DeQueuePortraitWidget()
{
	if(WidgetPool.IsEmpty())
	{
		WidgetPool.Enqueue(CreatePortraitWidget());
	}
	UCTurnOrderPortraitWidget* PortraitWidget;
	WidgetPool.Dequeue(PortraitWidget);
	return PortraitWidget;
}
void ACTurnOrderUIManager::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
}
