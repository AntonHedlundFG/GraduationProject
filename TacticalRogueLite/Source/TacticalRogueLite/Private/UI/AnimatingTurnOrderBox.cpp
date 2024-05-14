// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AnimatingTurnOrderBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Utility/Logging/CLogManager.h"

FLerpElementsToPositions::FLerpElementsToPositions(TArray<FVector2D> StartPositions,
	TArray<FVector2D> EndPositions, TArray<UCTurnOrderPortraitWidget*> Widgets,UCurveFloat* LerpCurve, float LerpTime,float WaitTimeAfterAnimationFinished)
{
	this->StartPositions = StartPositions;
	this->EndPositions = EndPositions;
	this->Widgets = Widgets;
	this->LerpCurve = LerpCurve;
	this->LerpTime = LerpTime;
	this->WaitTimeAfterAnimationFinished = WaitTimeAfterAnimationFinished;
}

bool FLerpElementsToPositions::Execute(float DeltaTime)
{
	FVector2D LerpedPosition = FVector2D::Zero();
	FVector2D Path = FVector2D::Zero();
	switch(State)
	{
		case 0:
			for (int i = 0; i < Widgets.Num(); i++)
			{
				Path = EndPositions[i] - StartPositions[i];
				LerpedPosition = StartPositions[i] + Path * ((LerpCurve != nullptr)?LerpCurve->GetFloatValue(Timer/LerpTime): Timer/LerpTime);
				Widgets[i]->SetPosition(LerpedPosition);
			}
			if(Timer >= LerpTime)
			{
				for (int i = 0; i < Widgets.Num(); i++)
				{
					Widgets[i]->SetPosition(EndPositions[i]); 
				}
				State = 1;
				Timer = 0;
				return false;
			}
			break;
		case 1:
			if(Timer >= WaitTimeAfterAnimationFinished)
			{
				return true;
			}
			break;
	}
	Timer+=DeltaTime;
	return false;
}

void FLerpElementsToPositions::OnStart()
{
	LOG_INFO("Starting Moving Task");
}

void FLerpElementsToPositions::OnEnd()
{
	LOG_INFO("Ending Moving Task");
}

int UAnimatingTurnOrderBox::AddWidget(UUserWidget* Widget)
{
	if(Widget == nullptr)
	{
		LOG(ELogCategory::LC_Error,"AnimatingTurnOrderBox: Trying to add an element that is null.");
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


void UAnimatingTurnOrderBox::UpdateOrder(TArray<int> NewOrder,UObject* Owner)
{
	if(NewOrder.Num() != ActiveWidgets.Num() || NewOrder.Num() == 0)
	{
		return;
	}

 	TArray<FVector2D> StartPositions;
	TArray<FVector2D> EndPositions;
	TArray<UCTurnOrderPortraitWidget*> TurnOrderPortraits;
	for (auto Child : ActiveWidgets)
	{
		if(Child == nullptr)
		{
			continue;
		}
		StartPositions.Add(GetWidgetCenterLocation(Child));
		TurnOrderPortraits.Add(Cast<UCTurnOrderPortraitWidget>(Child));
	}
	for (int i = 0; i< NewOrder.Num(); i++)
	{
		FVector2D EndPosition = WidgetOffset * NewOrder[i];
		EndPositions.Add(EndPosition);
	}

	FLerpElementsToPositions* Task = new FLerpElementsToPositions(StartPositions,EndPositions,TurnOrderPortraits,LerpCurve,LerpTime,WaitTimeAfterAnimationFinished);
	GetWorld()->GetSubsystem<UCCORExecutor>()->AddExecutable(Owner,Task);

	TArray<UUserWidget*> NewWidgetOrder;
	NewWidgetOrder.Init(nullptr,ActiveWidgets.Num());
	for(int i = 0; i < ActiveWidgets.Num(); i++)
	{
		NewWidgetOrder[NewOrder[i]] = ActiveWidgets[i];
	}
	ActiveWidgets = NewWidgetOrder;
}

FVector2D UAnimatingTurnOrderBox::GetWidgetCenterLocation(UUserWidget* Widget)
{
	return GetPanelSlot(Widget)->GetPosition();
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
