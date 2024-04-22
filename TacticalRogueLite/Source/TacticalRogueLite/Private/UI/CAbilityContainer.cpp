// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAbilityContainer.h"

#include "Items/CInventoryComponent.h"
#include "Utility/Logging/CLogManager.h"

void UCAbilityContainer::UpdateInfoTarget(FGameplayTag ItemSlot,UWidget* HoveredWidget,ACUnit* CurrentUnit)
{
	Panel->SetVisibility(ESlateVisibility::HitTestInvisible);
	UCItemData* ItemData = CurrentUnit->GetInventoryComp()->GetItemInSlot(ItemSlot);
	//LOG(ELogCategory::LC_Info,"Updated Target To: %s",ItemData->);
}

void UCAbilityContainer::ClearInfoTarget()
{
	Panel->SetVisibility(ESlateVisibility::Collapsed);
}

void UCAbilityContainer::NativeConstruct()
{
	Super::NativeConstruct();
	Panel->SetVisibility(ESlateVisibility::Collapsed);
}
