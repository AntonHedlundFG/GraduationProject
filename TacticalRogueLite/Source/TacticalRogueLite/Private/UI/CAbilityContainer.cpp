// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAbilityContainer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Items/CInventoryComponent.h"

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
	
	FString TitleText = ItemData->GetItemName().ToString();
	TitleText.Append(KeyButtonFromSlot(ItemSlot));
	TitleTextBox->SetText(FText::FromString(TitleText));
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

void UCAbilityContainer::UpdateInfoTargetWithCustomText(UWidget* HoveredWidget, FText Title, FText Description)
{
	InfoPanel->SetVisibility(ESlateVisibility::Visible);
	InfoPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
	TitleTextBox->SetText(Title);
	DescriptionTextBox->SetText(Description);
	HoverTarget = UWidgetLayoutLibrary::SlotAsCanvasSlot(HoveredWidget);
	InfoPanelSlot->SetPosition(HoverTarget->GetPosition() + FVector2D(HoverTarget->GetSize().X, -InfoPanelSlot->GetSize().Y));
}

FString UCAbilityContainer::KeyButtonFromSlot(FGameplayTag ItemSlot)
{
	FString Return = FString();
	if (ItemSlot.ToString().Contains("Boots"))
	{
		Return = FString(" (Q)");
	}
	else if (ItemSlot.ToString().Contains("Weapon"))
	{
		Return = FString(" (W)");
	}
	else if (ItemSlot.ToString().Contains("Armor"))
	{
		Return = FString(" (E)");
	}
	else if (ItemSlot.ToString().Contains("Trinket"))
	{
		Return = FString(" (R)");
	}
	return Return;
}
