#include "UI/CTurnOrderUI.h"

#include "CGameState.h"
#include "Actions/CActionComponent.h"
#include "GamePlayTags/SharedGameplayTags.h"
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
	//TODO:Ensure that the widget queue doesnt contain nullptrs
	
	if(WidgetPool.IsEmpty())
	{
		WidgetPool.Enqueue(CreatePortraitWidget());
	}
	UCTurnOrderPortraitWidget* Widget;
	WidgetPool.Dequeue(Widget);

	if(Widget == nullptr)
	{
		Widget = CreatePortraitWidget();
	}
	
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
//Jesus, jag tror att jag behöver göra en queue för allt det här också, nu kan det vara att man lägger till och tar bort index medans skiten håller på att processas
void UCTurnOrderUI::UpdateTurnList()
{
	TArray<ACUnit*> NewTurnOrder = GetWorld()->GetGameState<ACGameState>()->GetCurrentTurnOrder(true);
	
	//Check if any of the units are nullptr TODO: create a better solution for this
	for (ACUnit* Unit : NewTurnOrder)
	{
		if(Unit == nullptr)
		{
			LOG_WARNING("Unit in TurnOrder is nullptr");
			return;
		}
	}

	TArray<UCTurnOrderPortraitWidget*> PortraitsToAnimateIn;
	TArray<UCTurnOrderPortraitWidget*> PortraitsToAnimateOut;
	TArray<int> TurnOrder;
	TurnOrder.Init(0,NewTurnOrder.Num());
	TArray<int> IndiciesToRemove;

	//Remove all widgets we dont want to be reordered;
	for(int i = 0; i < LastTurnOrder.Num(); i++)
	{
		ACUnit* Unit = (LastTurnOrder)[i];

		if(!NewTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			PortraitsToAnimateOut.Add(Widget);
			TurnOrderBox->RemoveIndex(i);
			IndiciesToRemove.Add(i);
		}
	}

	//Reverse loop to avoid ordering issues.
	for (int i = IndiciesToRemove.Num() - 1; i >= 0; i--)
	{
		LastTurnOrder.RemoveAt(IndiciesToRemove[i]);
	}
	
	for (int i = 0; i < NewTurnOrder.Num(); i++)
	{
		ACUnit* Unit = NewTurnOrder[i];
		if (!IsValid(Unit)) continue;
		//Add null check to unit
		//We should add unit
		if(!LastTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = DeQueuePortraitWidget();
			if (!IsValid(Widget)) continue;
			FString UnitName = Unit->GetUnitName();
			FString Text = UnitName.IsEmpty() ? FString("Unknown") : UnitName;
			Widget->SetText(Text);
			ActivePortraits.Add(Unit,Widget);
			PortraitsToAnimateIn.Add(Widget);
			//Save the index in which it was added??
			int indexItWasAddedTo = TurnOrderBox->AddWidget(Widget);
			if (indexItWasAddedTo < TurnOrder.Num() && indexItWasAddedTo >= 0)
				TurnOrder[indexItWasAddedTo] = i;
			if(Unit->GetActionComp()->HasTag(TAG_Unit_IsPlayer))
			{
				Widget->SetBackground(PlayerBackgroundBrush);
			}
			else
			{
				Widget->SetBackground(EnemyBackgroundBrush);
			}
		}
		else if(LastTurnOrder.Contains(Unit))
		{
			for(int y = 0; y < LastTurnOrder.Num(); y++)
			{
				if(LastTurnOrder[y] == Unit)
				{
					TurnOrder[y] = i;
				}
			}
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
	LastTurnOrder = NewTurnOrder;
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
	UpdateTurnList();
}

void UCTurnOrderUI::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
	if(widget == nullptr) 
	{
		return;
	}
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
