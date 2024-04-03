// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CTurnOrderPortraitWidget.h"

void UCTurnOrderPortraitWidget::SetText(FString text)
{
	TextBlock->SetText(FText::FromString(text));
}

void UCTurnOrderPortraitWidget::SetPortrait(FSlateBrush Brush)
{
}

void UCTurnOrderPortraitWidget::SetPosition(FVector ViewportPosition)
{
}

void UCTurnOrderPortraitWidget::AnimateOut()
{
	PlayAnimation(OutAnimation);
}

void UCTurnOrderPortraitWidget::AnimateIn()
{
	PlayAnimation(InAnimation);
}

