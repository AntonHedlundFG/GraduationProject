// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CDropDownWidget.h"


void UCDropDownWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
	bOpen = false;
	Button->OnClicked.AddDynamic(this,&UCDropDownWidget::ToggleDropDownMenu);
}

void UCDropDownWidget::ToggleDropDownMenu()
{
	if(bOpen)
	{
		CanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		CanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}
	bOpen = !bOpen;
}
