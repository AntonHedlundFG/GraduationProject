#include "UI/TurnOrder/CTurnOrderUI.h"
#include "CGameState.h"
#include "Actions/CActionComponent.h"
#include "GamePlayTags/SharedGameplayTags.h"
#include "UI/TurnOrder/AnimatingTurnOrderBox.h"

void UCTurnOrderUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&UCTurnOrderUI::UpdateTurnList);
	}
	if(TurnOrderBox)
	{
		TurnOrderBox->InitializeSpacing(PortraitWidget);
	}
	Executor = GetWorld()->GetSubsystem<UCCORExecutor>();
	
	UpdateTurnList();
}

void UCTurnOrderUI::FetchNewTurnOrder(TArray<ACUnit*>& NewTurnOrder) const
{
	if (GameState)
	{
		NewTurnOrder = GameState->GetCurrentTurnOrder(true);

		for (auto It = NewTurnOrder.CreateIterator(); It; ++It)
		{
			if(*It == nullptr)
			{
				It.RemoveCurrent();
			}
		}
	}
}

void UCTurnOrderUI::UpdateTurnList()
{
	TArray<ACUnit*> NewTurnOrder;
	FetchNewTurnOrder(NewTurnOrder);
	UpdateUnitWidgetMap(NewTurnOrder);
	TurnOrderBox->UpdateOrder(UnitWidgets, this);
}


bool UCTurnOrderUI::ValidateUnitWidgetMap(const TArray<ACUnit*>& TurnOrder)
{
	if (UnitWidgets.Num() != TurnOrder.Num())
		return false;

	for (const ACUnit* Unit : TurnOrder)
	{
		if (!UnitWidgets.Contains(Unit))
			return false;
	}

	return true;
}

void UCTurnOrderUI::UpdateUnitWidgetMap(const TArray<ACUnit*>& TurnOrder)
{
	// Create a set of units present in the current UnitWidgets array for quick lookup
	TSet<ACUnit*> CurrentUnits;
	for (const FUnitWidget& UnitWidget : UnitWidgets)
	{
		CurrentUnits.Add(UnitWidget.Unit);
	}

	// Handle removal: Units to remove (present in the map but not in the turn order)
	for (auto It = UnitWidgets.CreateIterator(); It; ++It)
	{
		if (!TurnOrder.Contains(It->Unit))
		{
			HandleRemoval(It->Unit);
		}
	}

	// Units to add (present in the turn order but not in the map)
	for (ACUnit* Unit : TurnOrder)
	{
		if (!UnitWidgets.Contains(Unit))
		{
			HandleAddition(Unit);
		}
	}

	// Sort UnitWidgets in the new TurnOrder
	TArray<FUnitWidget> SortedUnitWidgets;
	for (ACUnit* Unit : TurnOrder)
	{
		const FUnitWidget* UnitWidgetPtr = UnitWidgets.FindByPredicate([Unit](const FUnitWidget& Widget)
		{
			return Widget.Unit == Unit;
		});
		if (UnitWidgetPtr)
		{
			SortedUnitWidgets.Add(*UnitWidgetPtr);
		}
	}

	UnitWidgets = SortedUnitWidgets;
}

void UCTurnOrderUI::HandleRemoval(ACUnit* Unit)
{
	FUnitWidget* UnitWidgetPtr = UnitWidgets.FindByPredicate([Unit](const FUnitWidget& Widget)
	{
		return Widget == Unit;
	});

	if(UnitWidgetPtr && UnitWidgetPtr->Widget)
	{
		UCTurnOrderPortraitWidget* Widget = UnitWidgetPtr->Widget;
		
		UAnimateOutPortraitWidgets_Executable* AnimateOutTask = NewObject<UAnimateOutPortraitWidgets_Executable>();
		AnimateOutTask->Initialize(Widget, this, TurnOrderBox, AnimationWaitTime);
		Executor->AddExecutable(this, AnimateOutTask);
		
		// Remove the widget from the array
		UnitWidgets.RemoveAll([Unit](const FUnitWidget& Widget) {
			return Widget == Unit;
		});

		TurnOrderBox->RemoveWidget(Widget);
	}
}

void UCTurnOrderUI::HandleAddition(ACUnit* Unit)
{
	if(!Unit) return;
	
	FString UnitName = Unit->GetUnitName();
	FString Text = UnitName.IsEmpty() ? FString("Unknown") : UnitName;

	UCTurnOrderPortraitWidget* Widget = CreateWidget<UCTurnOrderPortraitWidget>(this, PortraitWidget);
	if(!Widget) return;
	
	Widget->AddToViewport(-999);
	Widget->SetVisibility(ESlateVisibility::Collapsed);
	Widget->SetText(Text);

	FSlateBrush Brush = Unit->GetActionComp()->HasTag(TAG_Unit_IsPlayer) ? PlayerBackgroundBrush : EnemyBackgroundBrush;
	Widget->SetBackground(Brush);
	
	FUnitWidget NewWidget(Unit, Widget, ESlateVisibility::Collapsed, Text, Brush);
	UnitWidgets.Add(NewWidget);

	UAnimationInPortraitWidget_Executable* AnimateInTask = NewObject<UAnimationInPortraitWidget_Executable>();
	AnimateInTask->Initialize(Widget, AnimationWaitTime);
	Executor->AddExecutable(this, AnimateInTask);

	TurnOrderBox->AddWidget(Widget);
}

#pragma region Executables

void UAnimateOutPortraitWidgets_Executable::Initialize(const TObjectPtr<UCTurnOrderPortraitWidget> inPortrait,
	UCTurnOrderUI* inTurnOrderUI, UAnimatingTurnOrderBox* inTurnOrderBox, const float inWaitTimeAfterCompletion)
{
	Portrait = inPortrait;
	TurnOrderUI = inTurnOrderUI;
	TurnOrderBox = inTurnOrderBox;
	WaitTimeAfterCompletion = inWaitTimeAfterCompletion;
}

bool UAnimateOutPortraitWidgets_Executable::Execute(float DeltaTime)
{
	if(Timer >= WaitTimeAfterCompletion)
	{
		return true;
	}
	Timer += DeltaTime;
	return false;
}

void UAnimateOutPortraitWidgets_Executable::Start()
{
	UExecutable::Start();
	Portrait->AnimateOut();
}

void UAnimateOutPortraitWidgets_Executable::End()
{
	UExecutable::End();
	Portrait->RemoveFromParent();
	Portrait->SetVisibility(ESlateVisibility::Collapsed);
}

void UAnimationInPortraitWidget_Executable::Initialize(const TObjectPtr<UCTurnOrderPortraitWidget> inPortrait,
	const float inWaitTimeAfterCompletion)
{
	Portrait = inPortrait;
	WaitTimeAfterCompletion = inWaitTimeAfterCompletion;
}

bool UAnimationInPortraitWidget_Executable::Execute(float DeltaTime)
{
	if(Timer >= WaitTimeAfterCompletion)
	{
		return true;
	}
	Timer+= DeltaTime;
	return false;
}

void UAnimationInPortraitWidget_Executable::Start()
{
	UExecutable::Start();
	Portrait->SetVisibility(ESlateVisibility::Visible); 
	Portrait->AnimateIn();
}

void UAnimationInPortraitWidget_Executable::End()
{
	UExecutable::End();
}

#pragma endregion
