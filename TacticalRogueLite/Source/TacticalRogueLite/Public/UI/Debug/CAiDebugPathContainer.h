// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CAiDebugPathContainer.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAiDebugPathContainer : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta= (BindWidget))
	UTextBlock* TargetTextBlock;
	UPROPERTY(meta= (BindWidget))
	UTextBlock* AbilityTextBlock;

public:
	void SetTargetText(const FText& Text);
	void SetAbilityText(const FText& Text);
};
