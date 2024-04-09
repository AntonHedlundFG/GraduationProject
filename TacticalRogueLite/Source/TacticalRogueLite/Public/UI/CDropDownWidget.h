// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "CDropDownWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCDropDownWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanel;
public:
	bool bOpen;
	virtual void NativeConstruct() override;
	UFUNCTION()
	virtual void ToggleDropDownMenu();
};
