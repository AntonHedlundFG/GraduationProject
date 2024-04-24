// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/CAiDebugPathContainer.h"

void UCAiDebugPathContainer::SetTargetText(const FText& Text)
{
	TargetTextBlock->SetText(Text);
}

void UCAiDebugPathContainer::SetAbilityText(const FText& Text)
{
	AbilityTextBlock->SetText(Text);
}
