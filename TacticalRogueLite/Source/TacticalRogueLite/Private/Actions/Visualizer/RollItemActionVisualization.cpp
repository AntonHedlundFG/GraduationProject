// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/Visualizer/RollItemActionVisualization.h"

#include "CPlayerController.h"
#include "Actions/CActionComponent.h"
#include "Actions/CAction_RollItem.h"
#include "Widgets/CActorWidget.h"
#include "ItemData/CItemData.h"
#include "Kismet/GameplayStatics.h"

bool URollItemActionVisualization::CanVisualizeAction(UCAction* Action)
{
	return Action->IsA(UCAction_RollItem::StaticClass());
}

void URollItemActionVisualization::Enter()
{
	Super::Enter();
	
	ActionClass = Cast<UCAction_RollItem>(VisualizedAction);
	
	TArray<UCItemData*> Items = ActionClass->GetItems();

	UCActionComponent* ActionComponent = ActionClass->GetActionComp();
	if(ActionComponent)
	{
		if(ensure(WidgetClass))
		{
			ACUnit* Unit = Cast<ACUnit>(ActionClass->GetActionComp()->GetOuter());
			bool bIsOwning = Unit->IsControlledBy(UGameplayStatics::GetPlayerController(GetWorld(),0));
			ItemSelectionWindow = CreateWidget<UCItemSelectionWindow>(GetWorld(),WidgetClass);
			ItemSelectionWindow->AddToViewport();
			ItemSelectionWindow->UpdateInfo(*ActionClass,Items, bIsOwning,[this](UCItemData* ItemData){OnItemSelectedCallback(ItemData);});
			ItemSelectionWindow->Open();
		}
	}
}

bool URollItemActionVisualization::Tick(float DeltaTime)
{
	
	if (!ActionClass->IsRunning())
	{
		return true;
	}

	return false;
	
}

void URollItemActionVisualization::OnItemSelectedCallback(UCItemData* SelectedItem)
{
	ACPlayerController* PlayerController = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	ACUnit* Unit = Cast<ACUnit>(ActionClass->GetActionComp()->GetOuter());
	PlayerController->Server_EquipItem(Unit,SelectedItem,ActionClass);
	ItemSelectionWindow->Close();
	ItemSelectionWindow->RemoveFromParent();
}
