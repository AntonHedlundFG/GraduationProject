// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderPortraitWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UCTurnOrderPortraitWidget::SetText(FString text)
{
	TextBlock->SetText(FText::FromString(text));
}

void UCTurnOrderPortraitWidget::SetPortrait(FSlateBrush Brush)
{
}

void UCTurnOrderPortraitWidget::SetPosition(FVector2D ViewportPosition)
{
	 UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	 if (IsValid(PanelSlot))
		PanelSlot->SetPosition(ViewportPosition);
}

void UCTurnOrderPortraitWidget::SetBackground(FSlateBrush Brush)
{
	Background->SetBrush(Brush);
}

FVector2D UCTurnOrderPortraitWidget::GetSize()
{
	return FVector2D(Panel->GetWidthOverride(),Panel->GetHeightOverride());
}

FVector2D UCTurnOrderPortraitWidget::GetPosition()
{
	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	return PanelSlot->GetPosition();
}

void UCTurnOrderPortraitWidget::AnimateOut()
{
	PlayAnimation(OutAnimation);
}

void UCTurnOrderPortraitWidget::AnimateIn()
{
	PlayAnimation(InAnimation);
}

