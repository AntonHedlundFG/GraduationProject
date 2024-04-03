// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "CTurnOrderPortraitWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCTurnOrderPortraitWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* TextBlock;
	UPROPERTY(meta=(BindWidget))
	UImage* Portrait;

protected:
	FTimerHandle AnimateToPositionHandle;
	FTimerHandle AnimatedRemoveFromQueueHandle;

public:
	/*!
	 * Where the widget should animate to when it's object is being removed from the turn queue.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector RemoveFromQueueAnimationOffsetTarget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* RemoveFromQueueAnimationEasing;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* ToPositionAnimationEasing;

	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* OutAnimation;
	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* InAnimation;
	
	void SetText(FString Text);
	void SetPortrait(FSlateBrush Brush);
	void AnimateOut();
	void AnimateIn();
	void StartAnimationTimer_ToPosition();
};
