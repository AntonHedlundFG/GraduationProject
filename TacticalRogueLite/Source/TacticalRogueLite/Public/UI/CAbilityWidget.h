// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAbilityWidget.generated.h"

enum class EItemSlots : uint8;

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAbilityWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitiateAbilityUse(EItemSlots ItemSlot);

	UFUNCTION(BlueprintCallable)
	void UndoAbility();
	
};
