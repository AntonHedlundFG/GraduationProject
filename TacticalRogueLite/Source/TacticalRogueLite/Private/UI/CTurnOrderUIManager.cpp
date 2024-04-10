// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderUIManager.h"

#include "MeshAttributes.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"



FTurnOrderAnimationTask_Remove::FTurnOrderAnimationTask_Remove(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,float WaitTimeAfterCompletion = 0, float WaitTimeBetweenAnimations = 0)
{
	Portraits = AffectedPortraits;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
	State = 0;
}


bool FTurnOrderAnimationTask_Remove::Execute(float DeltaTime)
{
	switch (State)
	{
		default:return true;
		case 0:
			
			if(Timer == 0)
			{
				UCTurnOrderPortraitWidget* Widget = (Portraits)[0];
				Portraits.RemoveAt(0);
				Widget->AnimateOut();
			}
			if(Timer >= WaitTimeBetweenAnimations)
			{
				Timer = 0;
				if(Portraits.Num() == 0)
				{
					State = 1;
				}
				return false;
			}
			break;
		case 1:
			if(Timer > WaitTimeAfterCompletion)
			{
				State = 2;
			}
			break;
		case 2: return true;
	}
	Timer += DeltaTime;
	return false;
}

FTurnOrderAnimationTask_Add::FTurnOrderAnimationTask_Add(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits, TArray<FVector2D> Positions,
                                                         float WaitTimeAfterCompletion = 0, float WaitTimeBetweenAnimations = 0)
{
	
	Portraits = AffectedPortraits;
	this->Positions = Positions;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
	State = 0;
}



bool FTurnOrderAnimationTask_Add::Execute(float DeltaTime)
{
	switch (State)
	{
		case 0:
			if(Timer == 0)
			{
				if(Portraits.Num() == 0)
				{
					State = 1;
					break;
				}
				UCTurnOrderPortraitWidget* Widget = (Portraits)[0];
				Portraits.RemoveAt(0);
				Widget->SetPosition((Positions)[0]);
				Positions.RemoveAt(0);
				Widget->SetVisibility(ESlateVisibility::Visible);
				Widget->AnimateIn();
				UE_LOG(LogTemp,Warning,TEXT("AddingWidgetTOScreen"));
			}
			if(Timer >= WaitTimeBetweenAnimations)
			{
				Timer = 0;
				return false;
			}
			break;
		case 1:
			if(Timer >= WaitTimeAfterCompletion)
			{
				State = 2;
			}
			break;
		case 2:
			return true;
	}
	Timer += DeltaTime;
	return false;
}

FTurnOrderAnimationTask_MoveTo::FTurnOrderAnimationTask_MoveTo(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,
	TArray<FVector2D> Positions, float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations ,UCurveFloat* AnimationEasing)
{
	Portraits = AffectedPortraits;
	this->Positions = Positions;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
	this->WaitTimeBetweenAnimations = WaitTimeBetweenAnimations;
	this->AnimationEasing = AnimationEasing;
	State = 0;
}


bool FTurnOrderAnimationTask_MoveTo::Execute(float DeltaTime)
{
	FVector2D LerpedPosition;
	switch (State)
	{
		case 0:
			if(Timer == 0)
			{
				for (UCTurnOrderPortraitWidget* Portrait : Portraits)
				{
					StartPositions.Add(Portrait->GetPosition());
				}
			}
			for(int i = 0; i < Portraits.Num(); i++)
			{
				LerpedPosition = StartPositions[i] + (Positions[i] - StartPositions[i]) * AnimationEasing->GetFloatValue(Timer/WaitTimeBetweenAnimations);
				Portraits[i]->SetPosition(LerpedPosition);
			}
			if(Timer >=  WaitTimeBetweenAnimations)
			{
					Timer = 0;
					State = 1;
				return false;
			}
		break;
		case 1:
			if(Timer > WaitTimeAfterCompletion)
			{
				State = 2;
				return false;
			}
			break;
		case 2: return true;
		default: return true;
	}
	Timer += DeltaTime;
	return false;
}


FTurnOrderAnimationTask_EnqueueWidgets::FTurnOrderAnimationTask_EnqueueWidgets(
	TArray<UCTurnOrderPortraitWidget*> WidgetsToEnqueue, ACTurnOrderUIManager* TurnManager)
{
	this->WidgetsToEnqueue = WidgetsToEnqueue;
	this->TurnManager = TurnManager;
	State = 0;
}

bool FTurnOrderAnimationTask_EnqueueWidgets::Execute(float DeltaTime)
{
	for (UCTurnOrderPortraitWidget* Widget : Portraits)
	{
		TurnManager->EnQueuePortraitWidget(Widget);
	}
	return true;
}

ACTurnOrderUIManager::ACTurnOrderUIManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACTurnOrderUIManager::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&ACTurnOrderUIManager::UpdateTurnList);
	}

	Executor = GetWorld()->GetSubsystem<UCCORExecutor>();
	
}

/*void ACTurnOrderUIManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Tasks.IsEmpty())
	{
		return;
	}
	else
	{
		if(Tasks[0]->bHasStarted)
		{
			Tasks[0]->Start();
		}
		bool TaskFinished = Tasks[0]->Execute(DeltaSeconds);
		if(TaskFinished)
		{
			delete Tasks[0];
			Tasks.RemoveAt(0);
		}
	}
}*/

void ACTurnOrderUIManager::UpdateTurnList()
{
	UE_LOG(LogTemp,Warning,TEXT("UpdateTurnList Called"));
	TArray<ACUnit*>* NewTurnOrder =  &Cast<ACGameState>(GetWorld()->GetGameState())->TurnOrder;
	if(NewTurnOrder->Num() == 0 && LastTurnOrder.Num() == 0){return;}
	
	TArray<UCTurnOrderPortraitWidget*> WidgetsToAdd;
	TArray<FVector2D> AddPositions;
	TArray<UCTurnOrderPortraitWidget*> WidgetsToRemove;
	TArray<UCTurnOrderPortraitWidget*> WidgetsToMove;
	TArray<FVector2D> MovePositions;

	TArray<FVector2D> NewPositions = CalculateViewportPositions(NewTurnOrder->Num());

	TArray<ACUnit*> UnitQueue = TArray<ACUnit*>(*NewTurnOrder);

	for(int i = 0; i < NewTurnOrder->Num(); i++)
	{
		ACUnit* Unit = UnitQueue[0];
		UnitQueue.RemoveAt(0);

		if (!Unit) continue;

		if(!LastTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = DeQueuePortraitWidget();
			FString UnitName = Unit->GetUnitName();
			Widget->SetText(UnitName);
			ActivePortraits.Add(Unit,Widget);
			WidgetsToAdd.Add(Widget);
			AddPositions.Add(NewPositions[i]);
		}
		else if(i < LastTurnOrder.Num() && LastTurnOrder[i] != Unit)
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToMove.Add(Widget);
				MovePositions.Add(NewPositions[i]);
			}
			else
			{
				LOG(ELogCategory::LC_Error,"Trying to remove portrait Widget that doesn't exist.[CTurnOrderUIManager]");
			}
		}
		else if(i >= LastTurnOrder.Num())
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToMove.Add(Widget);
				MovePositions.Add(NewPositions[i]);
			}
		}
	}

	for(ACUnit* Unit: LastTurnOrder)
	{
		if(!NewTurnOrder->Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToRemove.Add(Widget);
				ActivePortraits.Remove(Unit);
			}
			else
			{
				LOG(ELogCategory::LC_Error,"Trying to remove portrait Widget that doesn't exist.[CTurnOrderUIManager]");
			}
		}
	}
	if(WidgetsToRemove.Num() != 0)
	{
		FTurnOrderAnimationTask_Remove* RemoveTask =  new FTurnOrderAnimationTask_Remove(WidgetsToRemove,AnimationWaitTime,AnimationTimeOffset);
		Executor->AddExecutable(this,RemoveTask);
		FTurnOrderAnimationTask_EnqueueWidgets* EnqueueTask = new FTurnOrderAnimationTask_EnqueueWidgets(WidgetsToRemove,this);
		Executor->AddExecutable(this,EnqueueTask);
	}
	if(WidgetsToMove.Num() != 0)
	{
		Algo::Reverse(WidgetsToMove);
		Algo::Reverse(MovePositions);
		FTurnOrderAnimationTask_MoveTo* MoveTask = new FTurnOrderAnimationTask_MoveTo(WidgetsToMove,MovePositions,AnimationWaitTime,MoveAnimationLerpTime,AnimationMoveToEasing);
		Executor->AddExecutable(this,MoveTask);
	}
	if(WidgetsToAdd.Num() != 0)
	{
		FTurnOrderAnimationTask_Add* AddTask = new FTurnOrderAnimationTask_Add(WidgetsToAdd,AddPositions,AnimationWaitTime,AnimationTimeOffset);
		Executor->AddExecutable(this,AddTask);
	}
	LastTurnOrder = TArray<ACUnit*>(*NewTurnOrder);
}

UCTurnOrderPortraitWidget* ACTurnOrderUIManager::CreatePortraitWidget()
{
	UCTurnOrderPortraitWidget* Widget = CreateWidget<UCTurnOrderPortraitWidget>(GetWorld()->GetGameInstance(),PortraitWidget);
	UE_LOG(LogTemp,Warning,TEXT("Creating new portrait widget"));
	Widget->AddToViewport(-999);
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
	widget->SetVisibility(ESlateVisibility::Hidden);
}



UCTurnOrderPortraitWidget* ACTurnOrderUIManager::GetActiveWidget(ACUnit* key)
{
	if(ActivePortraits.Contains(key))
	{
		return ActivePortraits[key];
	}
	return nullptr;
}

TArray<FVector2D> ACTurnOrderUIManager::CalculateViewportPositions(int AmountOfUnits)
{
	int32 ViewportX;
	int32 ViewportY;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->GetViewportSize(ViewportX,ViewportY);

	TArray<FVector2D> Positions;

	for(int i = 0; i < AmountOfUnits; i++)
	{
		int XPos = 0;
		int YPos = ViewportY * WidgetListStartPositionOffsetFromAnchor + (PortraitPixelOffset * i);
		Positions.Add(FVector2D(XPos,YPos));
	}
	
	return Positions;
}
