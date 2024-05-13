// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CItemDisplay.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GridContent/CUnit.h"
#include "CInventoryWindow.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCInventoryWindow : public UUserWidget
{
	GENERATED_BODY()
	/*UPROPERTY(meta = (BindWidget))
	UCItemDisplay* BootsDisplay;
	UPROPERTY(meta = (BindWidget))
	UCItemDisplay* WeaponDisplay;
	UPROPERTY(meta = (BindWidget))
	UCItemDisplay* ArmorDisplay;
	UPROPERTY(meta = (BindWidget))
	UCItemDisplay* TrinketDisplay;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ADText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ARText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MRText;*/
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInfo(ACUnit* Unit);
	UFUNCTION(BlueprintCallable)
	FString GetClassAsString(ACUnit* Unit);
};
