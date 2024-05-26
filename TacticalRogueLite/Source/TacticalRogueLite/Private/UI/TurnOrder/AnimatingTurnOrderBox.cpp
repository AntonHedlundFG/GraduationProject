#include "UI/TurnOrder/AnimatingTurnOrderBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/TurnOrder/CTurnOrderPortraitWidget.h"
#include "UI/TurnOrder/CTurnOrderUI.h"
#include "Utility/Logging/CLogManager.h"


void ULerpElementsToPositions::Initialize(const TArray<FVector2D>& InStartPositions,
	const TArray<FVector2D>& InEndPositions, const TArray<UCTurnOrderPortraitWidget*>& InWidgets,
	UCurveFloat* InLerpCurve, const float InLerpTime, const float InWaitTimeAfterAnimationFinished)
{
	
	StartPositions = InStartPositions;
	EndPositions = InEndPositions;
	Widgets = InWidgets;
	LerpCurve = InLerpCurve;
	LerpTime = InLerpTime;
	WaitTimeAfterAnimationFinished = InWaitTimeAfterAnimationFinished;
	
	for (int i = 0; i < StartPositions.Num(); ++i)
	{
		LOG_INFO("StartPosition: %s EndPosition: %s", *StartPositions[i].ToString(), *EndPositions[i].ToString());
	}
}

bool ULerpElementsToPositions::Execute(float DeltaTime)
{
	LOG_INFO("ULerpElementsToPositions");
	
	switch(State)
	{
	case EExecutableState::NotStarted:
		return true;
	case EExecutableState::Paused:
		return true;
	case EExecutableState::Running:
		
			for (int i = 0; i < Widgets.Num(); i++)
			{
				FVector2D Path = EndPositions[i] - StartPositions[i];
				FVector2D LerpedPosition = StartPositions[i];
				FVector2D PathDelta = Path * (LerpCurve != nullptr ? LerpCurve->GetFloatValue(Timer / LerpTime) : Timer / LerpTime);
				LerpedPosition += PathDelta;
				Widgets[i]->SetPosition(LerpedPosition);
			}
		
			if(Timer >= LerpTime)
			{
				for (int i = 0; i < Widgets.Num(); i++)
				{
					Widgets[i]->SetPosition(EndPositions[i]); 
				}
				State = EExecutableState::Completed;
				Timer = 0;
			}
		
			break;
		case EExecutableState::Completed:
			if(Timer >= WaitTimeAfterAnimationFinished)
			{
				return true;
			}
			break;
		default:
			break;
	}
	
	Timer += DeltaTime;
	return false;
}

int UAnimatingTurnOrderBox::AddWidget(UUserWidget* Widget)
{
	if(Widget == nullptr)
	{
		LOG_ERROR("AnimatingTurnOrderBox: Trying to add an element that is null.");
	}
	ActiveWidgets.Add(Widget);
	AddChildToCanvas(Widget);
	UCanvasPanelSlot* PanelSlot = GetPanelSlot(Widget);
	PanelSlot->SetAutoSize(true);
	ChildAlignment.Anchors.Minimum = {1,0};
	ChildAlignment.Anchors.Maximum = {1,0};
	PanelSlot->SetAnchors(ChildAlignment.Anchors);
	ChildAlignment.Alignment = {1,0};
	PanelSlot->SetAlignment(ChildAlignment.Alignment);
	PanelSlot->SetPosition(GetNextPosition());
	
	return ActiveWidgets.Num()-1;
}

void UAnimatingTurnOrderBox::RemoveIndex(int index)
{
	if(index >= ActiveWidgets.Num() || index < 0)
	{
		return;
	}
	ActiveWidgets.RemoveAt(index);
}

void UAnimatingTurnOrderBox::RemoveWidget(UUserWidget* Widget)
{
	int Index = ActiveWidgets.IndexOfByKey(Widget);
	RemoveIndex(Index);
	RemoveChild(Widget);
}


void UAnimatingTurnOrderBox::UpdateOrder(const TArray<FUnitWidget>& UnitWidgetsTurnOrder, UObject* Owner)
{
	if(UnitWidgetsTurnOrder.Num() != ActiveWidgets.Num() || UnitWidgetsTurnOrder.Num() == 0)
	{
		return;
	}
	
	// Update ActiveWidget to the new order
	TArray<UUserWidget*> NewWidgetOrder;
	NewWidgetOrder.Init(nullptr,ActiveWidgets.Num());
	for(int i = 0; i < ActiveWidgets.Num(); i++)
	{
		const FUnitWidget& UnitWidget = UnitWidgetsTurnOrder[i];
		NewWidgetOrder[i] = UnitWidget.Widget;
	}
	ActiveWidgets = NewWidgetOrder;
	
 	TArray<FVector2D> StartPositions;
	TArray<FVector2D> EndPositions;
	TArray<UCTurnOrderPortraitWidget*> TurnOrderPortraits;
	
	// Fill StartPositions and TurnOrderPortraits
	for (auto Child : ActiveWidgets)
	{
		if(!Child) continue;
		
		StartPositions.Add(GetWidgetCenterLocation(Child));
		TurnOrderPortraits.Add(Cast<UCTurnOrderPortraitWidget>(Child));
	}
	
	// Calculate EndPositions based on UnitWidgetsTurnOrder
	for (int i = 0; i < UnitWidgetsTurnOrder.Num(); i++)
	{
		FVector2D EndPosition = WidgetOffset * i;
		EndPositions.Add(EndPosition);
	}
	
	// Create the animation task
	ULerpElementsToPositions* Task = NewObject<ULerpElementsToPositions>(this);
	Task->Initialize(StartPositions, EndPositions, TurnOrderPortraits, LerpCurve, LerpTime, WaitTimeAfterAnimationFinished);
	GetWorld()->GetSubsystem<UCCORExecutor>()->AddExecutable(Owner, Task);
}

FVector2D UAnimatingTurnOrderBox::GetWidgetCenterLocation(UUserWidget* Widget)
{
	FVector2D Pos = FVector2D::ZeroVector;
	if( Widget )
	{
		Pos = GetPanelSlot(Widget)->GetPosition();
	}
	return Pos;
}

FVector2D UAnimatingTurnOrderBox::GetNextPosition()
{
	int NumElements = ActiveWidgets.Num();
	return NumElements * WidgetOffset;
}

UCanvasPanelSlot* UAnimatingTurnOrderBox::GetPanelSlot(UUserWidget* Widget)
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget);
}

void UAnimatingTurnOrderBox::InitializeSpacing(TSubclassOf<UUserWidget> Prefab)
{
	UWidget* Child = CreateWidget(GetWorld()->GetGameInstance(),Prefab);
	if(UCTurnOrderPortraitWidget* TurnOrderPortrait = Cast<UCTurnOrderPortraitWidget>(Child))
	{
		FVector2D PanelSize = TurnOrderPortrait->GetSize();
		WidgetOffset = FVector2D(0,PanelSize.Y);
	}
	else
	{
		AddChild(Child);
		UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Child);
		PanelSlot->SetAutoSize(true);
		//This is not it, i just dont know how to get the automatic size after its translated
		FVector2D PanelSize = PanelSlot->GetSize();
		WidgetOffset = FVector2D(0,PanelSize.Y);
		RemoveChild(Child);	
	}
}
