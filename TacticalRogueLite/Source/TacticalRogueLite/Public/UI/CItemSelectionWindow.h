// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CInventoryWindow.h"
#include "CItemSelectionButton.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "GridContent/CUnit.h"
#include "CItemSelectionWindow.generated.h"

class UCAction_RollItem;
class UCItemData;


UCLASS()
class TACTICALROGUELITE_API UCItemSelectionWindow : public UUserWidget
{
	GENERATED_BODY()
	bool bOwner = false;
	UPROPERTY()
	TArray<UCItemSelectionButton*> ActiveItemSelectionWidgets;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;
	UPROPERTY(meta = (BindWidget))
	UCInventoryWindow* Inventory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCItemSelectionButton> ItemSelectionButtonWidget;
	TFunction<void(UCItemData* ItemData)> OnItemSelectedCallBack;
	void OnItemSelected(UCItemData* SelectedItem);
	void ClearScrollBox();
public:
	void UpdateInfo(UCAction_RollItem& Action, TArray<UCItemData*> Items, bool bIsOwner, TFunction<void(UCItemData* ItemData)>CallBack);
	UFUNCTION(BlueprintImplementableEvent, Category = "CPP")
	void UpdateInfo_BP(bool bIsOwner);
	void Open();
	void Close();
};
