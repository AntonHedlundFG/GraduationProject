// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CItemSelectionButton.h"

void UCItemSelectionButton::OnButtonClicked()
{
	if(OnItemSelectedCallBack != nullptr)
	{
		OnItemSelectedCallBack(CurrentItem);
	}
}
void UCItemSelectionButton::SetInteractable(bool bIsInteractable)
{
	Button->SetVisibility(bIsInteractable?ESlateVisibility::Visible:ESlateVisibility::HitTestInvisible);
}

void UCItemSelectionButton::UpdateInfo(UCItemData* Item)
{
	CurrentItem = Item;
	Icon->SetBrushFromTexture(Item->GetIconAsTexture().Get());
	Name->SetText(Item->GetItemName());
	Description->SetText(Item->GetDescription());
	Button->OnClicked.AddUniqueDynamic(this,&UCItemSelectionButton::OnButtonClicked);
}

void UCItemSelectionButton::BindDelegateOnItemSelected(TFunction<void(UCItemData* ItemData)> Callback)
{
	OnItemSelectedCallBack = Callback;
}


void UCItemSelectionButton::UnBindDelegateOnItemSelected()
{
	OnItemSelectedCallBack = nullptr;
}

