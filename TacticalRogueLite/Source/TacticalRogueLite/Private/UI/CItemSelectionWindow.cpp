// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CItemSelectionWindow.h"

void UCItemSelectionWindow::UpdateInfo(const UCAction& Action,TArray<UCItemData*> Items,bool bIsOwner, TFunction<void(UCItemData* ItemData)>CallBack = nullptr)
{
	this->bIsOwner = bIsOwner;
	OnItemSelectedCallBack = CallBack;
	ClearScrollBox();
	for(auto data : Items)
	{
		UCItemSelectionButton* Button = CreateWidget<UCItemSelectionButton>(this,ItemSelectionButtonWidget);
		Button->AddToViewport();
		Button->SetInteractable(bIsOwner);
		if(bIsOwner)
		{
			Button->BindDelegateOnItemSelected([this](UCItemData* ItemData){OnItemSelected(ItemData);});
		}
		Button->UpdateInfo(data);
		ActiveItemSelectionWidgets.Add(Button);
		ScrollBox->AddChild(Button);
	}
}

void UCItemSelectionWindow::OnItemSelected(UCItemData* SelectedItem)
{
	OnItemSelectedCallBack(SelectedItem);
	OnItemSelectedCallBack = nullptr;
	ClearScrollBox();
	bIsOwner = false;
}

void UCItemSelectionWindow::ClearScrollBox()
{
	for(auto ItemSelectionButton : ActiveItemSelectionWidgets)
	{
		ItemSelectionButton->UnBindDelegateOnItemSelected();
		ScrollBox->RemoveChild(ItemSelectionButton);
	}
	ActiveItemSelectionWidgets.Empty();
}
void UCItemSelectionWindow::Open()
{
	SetVisibility(bIsOwner?ESlateVisibility::Visible:ESlateVisibility::HitTestInvisible);
}

void UCItemSelectionWindow::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}
