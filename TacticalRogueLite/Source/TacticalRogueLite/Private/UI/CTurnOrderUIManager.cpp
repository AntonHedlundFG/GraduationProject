// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderUIManager.h"

FTurnOrderAnimationTask::~FTurnOrderAnimationTask()
{
}

FTurnOrderAnimationTask_Remove::FTurnOrderAnimationTask_Remove(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,float WaitTimeAfterCompletion = 0, float WaitTimeBetweenAnimations = 0)
{
	Portraits = AffectedPortraits;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
}


bool FTurnOrderAnimationTask_Remove::Execute()
{
	return FTurnOrderAnimationTask::Execute();
}

FTurnOrderAnimationTask_Add::FTurnOrderAnimationTask_Add(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits, TArray<FVector2D> Positions,
                                                         float WaitTimeAfterCompletion = 0, float WaitTimeBetweenAnimations = 0)
{
	Portraits = AffectedPortraits;
	this->Positions = Positions;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
}



bool FTurnOrderAnimationTask_Add::Execute()
{
	return FTurnOrderAnimationTask::Execute();
}

FTurnOrderAnimationTask_MoveTo::FTurnOrderAnimationTask_MoveTo(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,
	TArray<FVector2D> Positions, float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations)
{
	Portraits = AffectedPortraits;
	this->Positions = Positions;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
}


bool FTurnOrderAnimationTask_MoveTo::Execute()
{
	return FTurnOrderAnimationTask::Execute();
}

void ACTurnOrderUIManager::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&ACTurnOrderUIManager::UpdateTurnList);
	}
}

void ACTurnOrderUIManager::UpdateTurnList()
{
	TArray<ACUnit*>* NewTurnOrder =  &Cast<ACGameState>(GetWorld()->GetGameState())->TurnOrder;
}

UCTurnOrderPortraitWidget* ACTurnOrderUIManager::CreatePortraitWidget()
{
	UCTurnOrderPortraitWidget* Widget = CreateWidget<UCTurnOrderPortraitWidget>(GetWorld()->GetGameInstance(),PortraitWidget);
	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Hidden);
	return Widget;
}
UCTurnOrderPortraitWidget* ACTurnOrderUIManager::DeQueuePortraitWidget()
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
void ACTurnOrderUIManager::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
	HandleEnqueue(widget);
	WidgetPool.Enqueue(widget);
}

void ACTurnOrderUIManager::HandleEnqueue(UCTurnOrderPortraitWidget* widget)
{
	widget->SetVisibility(ESlateVisibility::Hidden);
}

void ACTurnOrderUIManager::HandleDequeue(UCTurnOrderPortraitWidget* widget)
{
	widget->SetVisibility(ESlateVisibility::Visible);
}

TArray<FVector2D> ACTurnOrderUIManager::CalculateVieportPositionOfPortrait(int AmountOfUnits)
{
	return TArray<FVector2D>();
}
