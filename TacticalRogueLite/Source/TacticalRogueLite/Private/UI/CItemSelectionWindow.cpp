// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CItemSelectionWindow.h"
#include "Actions/CActionComponent.h"


void UCItemSelectionWindow::UpdateInfo(UCAction_RollItem& Action,TArray<UCItemData*> Items,bool bIsOwner, TFunction<void(UCItemData* ItemData)>CallBack = nullptr)
{
	this->bOwner = bIsOwner;
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
	ACUnit* Unit = Cast<ACUnit>(Action.GetActionComp()->GetOuter());
	Inventory->UpdateInfo(Unit);
	
	UpdateInfo_BP(bIsOwner);

	if(bIsOwner)
	{
		SkipItemButton->SetVisibility(ESlateVisibility::Visible);
		SkipItemButton->OnClicked.AddUniqueDynamic(this,&UCItemSelectionWindow::SkipItem);
	}
	else
	{
		SkipItemButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCItemSelectionWindow::OnItemSelected(UCItemData* SelectedItem)
{
	if(OnItemSelectedCallBack == nullptr)
	{
		return;
	}
	OnItemSelectedCallBack(SelectedItem);
	OnItemSelectedCallBack = nullptr;
	ClearScrollBox();
	bOwner = false;
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

void UCItemSelectionWindow::SkipItem()
{
	OnItemSelected(nullptr);
}

void UCItemSelectionWindow::Open()
{
	SetVisibility(bOwner?ESlateVisibility::Visible:ESlateVisibility::HitTestInvisible);
}

void UCItemSelectionWindow::Close()
{
	bClosing = true;
}

void UCItemSelectionWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bClosing)
	{
		closingTime += InDeltaTime;
		float Alpha = FMath::Clamp(1.0f - closingTime, 0.0f, 1.0f);
		SetRenderOpacity(Alpha);
		if (Alpha == 0.0f)
		{
			bClosing = false;
			SetVisibility(ESlateVisibility::Hidden);
			RemoveFromParent();
		}
	}
}
