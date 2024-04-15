// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderUI.h"

#include "CGameState.h"
#include "Utility/Logging/CLogManager.h"

UCTurnOrderPortraitWidget* UCTurnOrderUI::CreatePortraitWidget()
{
	UCTurnOrderPortraitWidget* Widget = CreateWidget<UCTurnOrderPortraitWidget>(GetWorld()->GetGameInstance(),PortraitWidget);
	UE_LOG(LogTemp,Warning,TEXT("Creating new portrait widget"));
	Widget->AddToViewport(-999);
	Widget->SetVisibility(ESlateVisibility::Hidden);
	return Widget;
}

UCTurnOrderPortraitWidget* UCTurnOrderUI::DeQueuePortraitWidget()
{
	if(WidgetPool.IsEmpty())
	{
		WidgetPool.Enqueue(CreatePortraitWidget());
	}
	UCTurnOrderPortraitWidget* Widget;
	WidgetPool.Dequeue(Widget);
	HandleDequeue(Widget);
	return Widget;
}

void UCTurnOrderUI::HandleEnqueue(UCTurnOrderPortraitWidget* widget)
{
	widget->SetVisibility(ESlateVisibility::Collapsed);
}

void UCTurnOrderUI::HandleDequeue(UCTurnOrderPortraitWidget* widget)
{
}

UCTurnOrderPortraitWidget* UCTurnOrderUI::GetActiveWidget(ACUnit* key)
{
	if(ActivePortraits.Contains(key))
	{
		return ActivePortraits[key];
	}
	return nullptr;
}

void UCTurnOrderUI::UpdateTurnList()
{
	LOG(ELogCategory::LC_Gameplay,"Im trying to update turn order list");
}

void UCTurnOrderUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&UCTurnOrderUI::UpdateTurnList);
	}
}

void UCTurnOrderUI::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
	HandleEnqueue(widget);
	WidgetPool.Enqueue(widget);
}
