// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AnimatingTurnOrderBox.h"

void FLerpElementsToPositions::OnStart()
{
}

void UAnimatingTurnOrderBox::UpdateOrder(TArray<int> NewOrder)
{
	FChildren* Children = MyVerticalBox->GetChildren();
	if(NewOrder.Num() != Children->Num())
	{
		return;
	}

	TArray<FVector2D> StartPositions;

	for (auto StartPosition : StartPositions)
	{
		
	}
}

FVector2D UAnimatingTurnOrderBox::GetWidgetCenterLocation(UWidget* Widget)
{
	FGeometry Geometry = GetCachedGeometry();
	FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;
	return Position;
}
