// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/Visualizer/RollItemActionVisualization.h"
#include "Actions/CAction_RollItem.h"
#include "Widgets/CActorWidget.h"
#include "ItemData/CItemData.h"

bool URollItemActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return Action->IsA(UCAction_RollItem::StaticClass());
}

void URollItemActionVisualization::Enter_Implementation()
{
	Super::Enter_Implementation();

	if (!ensure(ActionClass)) return;
	
	ActionClass = Cast<UCAction_RollItem>(VisualizedAction);
	
	TArray<UCItemData*> Items = ActionClass->GetItems();

	UCActionComponent* ActionComponent = ActionClass->GetActionComp();
	if(ActionComponent)
	{
		if(ensure(Widget))
		{
			//Init Widget...?
			//Widget->UpdateInfo(Items, ActionComponent);
		}
	}
}

bool URollItemActionVisualization::Tick_Implementation(float DeltaTime)
{
	return Super::Tick_Implementation(DeltaTime);
	
}
