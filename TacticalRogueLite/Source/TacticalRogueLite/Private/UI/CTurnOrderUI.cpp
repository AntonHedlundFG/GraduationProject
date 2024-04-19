// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderUI.h"

#include "CGameState.h"
#include "DiffUtils.h"
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
//Im going to write something super hacky 
void UCTurnOrderUI::UpdateTurnList()
{
	TArray<ACUnit*>* NewTurnOrder =  &Cast<ACGameState>(GetWorld()->GetGameState())->TurnOrder;

	TArray<UCTurnOrderPortraitWidget*> PortraitsToAnimateIn;
	TArray<UCTurnOrderPortraitWidget*> PortraitsToAnimateOut;
	TArray<int> TurnOrder;
	TurnOrder.Init(0,NewTurnOrder->Num());

	//Remove all widgets we dont want to be reordered;
	for(int i = 0; i < LastTurnOrder.Num(); i++)
	{
		ACUnit* Unit = (LastTurnOrder)[i];

		if(!NewTurnOrder->Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			PortraitsToAnimateOut.Add(Widget);
			TurnOrderBox->RemoveIndex(i);
		}
	}
	for (int i = 0; i < NewTurnOrder->Num(); i++)
	{
		ACUnit* Unit = (*NewTurnOrder)[i];
		//We should add unit
		if(!LastTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = DeQueuePortraitWidget();
			Widget->SetText(Unit->GetUnitName());
			ActivePortraits.Add(Unit,Widget);
			PortraitsToAnimateIn.Add(Widget);
			//Save the index in which it was added??
			int indexItWasAddedTo = TurnOrderBox->AddWidget(Widget);
			TurnOrder[indexItWasAddedTo] = i;
		}
		//We should reorder unit
		else
		{
			TurnOrder[LastTurnOrder_UnitToIndex[Unit]] = i;
		}
	}
	if(PortraitsToAnimateOut.Num() > 0)
	{
		FAnimateOutPortraitWidgets_Executable* AnimateOutAndRemoveTask = new FAnimateOutPortraitWidgets_Executable(PortraitsToAnimateOut,this,TurnOrderBox,AnimationWaitTime);
		Executor->AddExecutable(this,AnimateOutAndRemoveTask);
	}
	if(PortraitsToAnimateIn.Num() > 0)
	{
		FAnimationInPortraitWidget_Executable* AnimateInTask = new FAnimationInPortraitWidget_Executable(PortraitsToAnimateIn,AnimationWaitTime);
		Executor->AddExecutable(this,AnimateInTask);
	}
	
	TurnOrderBox->UpdateOrder(TurnOrder,this);
	LastTurnOrder = TArray<ACUnit*>(*NewTurnOrder);
	LastTurnOrder_UnitToIndex.Empty();
	for (int i = 0; i < LastTurnOrder.Num(); i++)
	{
		LastTurnOrder_UnitToIndex.Add(LastTurnOrder[i],i);
	}
}

void UCTurnOrderUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&UCTurnOrderUI::UpdateTurnList);
	}
	TurnOrderBox->InitializeSpacing(PortraitWidget);
	Executor = GetWorld()->GetSubsystem<UCCORExecutor>();
}

void UCTurnOrderUI::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
	HandleEnqueue(widget);
	WidgetPool.Enqueue(widget);
}
#pragma region Executables

FAnimateOutPortraitWidgets_Executable::FAnimateOutPortraitWidgets_Executable(TArray<UCTurnOrderPortraitWidget*> Portraits, UCTurnOrderUI* TurnOrderUI, UAnimatingTurnOrderBox* TurnOrderBox, float WaitTimeAfterCompletion)
{
	this->Portraits = Portraits;
	this->TurnOrderUI = TurnOrderUI;
	this->TurnOrderBox = TurnOrderBox;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
}

bool FAnimateOutPortraitWidgets_Executable::Execute(float DeltaTime)
{
	if(Timer >= WaitTimeAfterCompletion)
	{
		return true;
	}
	Timer += DeltaTime;
	return false;
}

void FAnimateOutPortraitWidgets_Executable::OnStart()
{
	FExecutable::OnStart();
	for(auto Portrait: Portraits)
	{
		Portrait->AnimateOut();
	}
}

void FAnimateOutPortraitWidgets_Executable::OnEnd()
{
	FExecutable::OnEnd();
	for(auto Portrait: Portraits)
	{
		TurnOrderBox->RemoveChild(Portrait);
		TurnOrderUI->EnQueuePortraitWidget(Portrait);
	}
}

FAnimationInPortraitWidget_Executable::FAnimationInPortraitWidget_Executable(TArray<UCTurnOrderPortraitWidget*> Portraits,
	float WaitTimeAfterCompletion)
{
	this->Portraits = Portraits;
	this->WaitTimeAfterCompletion = WaitTimeAfterCompletion;
}

bool FAnimationInPortraitWidget_Executable::Execute(float DeltaTime)
{
	if(Timer >= WaitTimeAfterCompletion)
	{
		return true;
	}
	Timer+= DeltaTime;
	return false;
}

void FAnimationInPortraitWidget_Executable::OnStart()
{
	FExecutable::OnStart();
	for(auto Portrait: Portraits)
	{
		Portrait->SetVisibility(ESlateVisibility::Visible); 
		Portrait->AnimateIn();
	}
}

void FAnimationInPortraitWidget_Executable::OnEnd()
{
	FExecutable::OnEnd();
	
}
#pragma endregion
