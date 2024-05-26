// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "CTurnOrderPortraitWidget.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCTurnOrderPortraitWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* TextBlock;
	UPROPERTY(meta=(BindWidget))
	UImage* Portrait;
	UPROPERTY(meta=(BindWidget))
	USizeBox* Panel;
	UPROPERTY(meta=(BindWidget))
	UImage* Background;
public:

	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* OutAnimation;
	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* InAnimation;
	
	void SetText(FString Text);
	void SetPortrait(FSlateBrush Brush);
	void SetPosition(FVector2D ViewportPosition);
	void SetBackground(FSlateBrush Brush);
	FVector2D GetSize();
	FVector2D GetPosition();
	void AnimateOut();
	void AnimateIn();
};
