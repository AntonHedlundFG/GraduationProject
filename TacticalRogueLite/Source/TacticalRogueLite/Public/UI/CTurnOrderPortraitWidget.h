// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
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
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* Panel;
public:
	/*!
	 * Where the widget should animate to when it's object is being removed from the turn queue.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector RemoveFromQueueAnimationOffsetTarget;

	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* OutAnimation;
	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* InAnimation;
	
	void SetText(FString Text);
	void SetPortrait(FSlateBrush Brush);
	void SetPosition(FVector2D ViewportPosition);
	FVector2D GetPosition();
	void AnimateOut();
	void AnimateIn();
};
