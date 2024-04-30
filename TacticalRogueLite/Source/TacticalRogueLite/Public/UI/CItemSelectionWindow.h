// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CItemSelectionButton.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "CItemSelectionWindow.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCItemSelectionWindow : public UUserWidget
{
	GENERATED_BODY()
	bool bIsOwner = false;
	UPROPERTY()
	TArray<UCItemSelectionButton*> ActiveItemSelectionWidgets;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCItemSelectionButton> ItemSelectionButtonWidget;
	TFunction<void(UCItemData* ItemData)> OnItemSelectedCallBack;
	void OnItemSelected(UCItemData* SelectedItem);
	void ClearScrollBox();
public:
	void UpdateInfo(const UCAction& Action,TArray<UCItemData*> Items,bool bIsOwner, TFunction<void(UCItemData* ItemData)>CallBack);
	void Open();
	void Close();
};
