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
	 UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Panel);
	 PanelSlot->SetPosition(ViewportPosition);
}

void UCTurnOrderPortraitWidget::AnimateOut()
{
	PlayAnimation(OutAnimation);
}

void UCTurnOrderPortraitWidget::AnimateIn()
{
	PlayAnimation(InAnimation);
}

