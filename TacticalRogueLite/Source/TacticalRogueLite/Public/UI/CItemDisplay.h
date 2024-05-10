// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction_RollItem.h"
#include "Blueprint/UserWidget.h"
#include "CItemDisplay.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCItemDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateInfo(UCItemData* ItemData);
};
