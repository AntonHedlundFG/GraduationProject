// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RichTextBlock.h"
#include "GridContent/CUnit.h"
#include "Items/ItemSlots.h"
#include "CAbilityContainer.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAbilityContainer : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UWidget* InfoPanel;
	UPROPERTY(meta = (BindWidget))
	URichTextBlock* TitleTextBox;
	UPROPERTY(meta = (BindWidget))
	URichTextBlock* DescriptionTextBox;

	UPROPERTY()
	UCanvasPanelSlot* HoverTarget;
	UPROPERTY()
	UCanvasPanelSlot* InfoPanelSlot;

	UFUNCTION(BlueprintCallable)
	void UpdateInfoTarget(FGameplayTag ItemSlot,UWidget* HoveredWidget,ACUnit* CurrentUnit);
	UFUNCTION(BlueprintCallable)
	void ClearInfoTarget();
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateInfoTargetWithCustomText(UWidget* HoveredWidget, FText Title, FText Description);

	FString KeyButtonFromSlot(FGameplayTag ItemSlot);
};
