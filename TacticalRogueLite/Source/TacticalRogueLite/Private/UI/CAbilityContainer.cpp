// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAbilityContainer.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Items/CInventoryComponent.h"
#include "Utility/Logging/CLogManager.h"

void UCAbilityContainer::UpdateInfoTarget(FGameplayTag ItemSlot,UWidget* HoveredWidget,ACUnit* CurrentUnit)
{
	if(HoveredWidget == nullptr || CurrentUnit == nullptr)
	{
		return;
	}
	UCItemData* ItemData = CurrentUnit->GetInventoryComp()->GetItemInSlot(ItemSlot);
	if(!ItemData)
	{
		return;
	}
	InfoPanel->SetVisibility(ESlateVisibility::Visible);
	InfoPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
	TitleTextBox->SetText(ItemData->GetItemName());
	DescriptionTextBox->SetText(ItemData->GetDescription());
	HoverTarget = UWidgetLayoutLibrary::SlotAsCanvasSlot(HoveredWidget);
	InfoPanelSlot->SetPosition(HoverTarget->GetPosition() + FVector2D(HoverTarget->GetSize().X,-InfoPanelSlot->GetSize().Y));
}

void UCAbilityContainer::ClearInfoTarget()
{
	InfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	HoverTarget = nullptr;
}

void UCAbilityContainer::NativeConstruct()
{
	Super::NativeConstruct();
	InfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	InfoPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InfoPanel);
}
