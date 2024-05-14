// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GridContent/CUnit.h"
#include "ItemData/CItemData.h"
#include "CItemSelectionButton.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCItemSelectionButton : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta =(BindWidget))
	UButton* Button;
	UPROPERTY(meta =(BindWidget))
	UTextBlock* Name;
	UPROPERTY(meta =(BindWidget))
	UTextBlock* Description;
	UPROPERTY(meta =(BindWidget))
	UTextBlock* SlotText;
	UPROPERTY(meta =(BindWidget))
	UImage* Icon;
	UPROPERTY(EditAnywhere)
	FSlateBrush DefaultIconBrush;
	UPROPERTY()
	UCItemData* CurrentItem;
	TFunction<void(UCItemData* ItemData)> OnItemSelectedCallBack;
	UFUNCTION()
	void OnButtonClicked();
public:
	void SetInteractable(bool bIsInteractable);
	void UpdateInfo(UCItemData* Item);
	void BindDelegateOnItemSelected(TFunction<void(UCItemData* ItemData)> Callback);
	void UnBindDelegateOnItemSelected();
};
