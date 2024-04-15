// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTurnOrderPortraitWidget.h"
#include "Components/VerticalBox.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "AnimatingTurnOrderBox.generated.h"

/**
 * 
 */

struct FLerpElementsToPositions: public FExecutable
{
	TArray<int> UpdatedOrder;
	TArray<FVector2D> StartPositions;
	TArray<FVector2D> EndPositions;
	TArray<UWidget*> Widgets;
	FLerpElementsToPositions(TArray<int> UpdatedOrder,TArray<FVector2D> StartPositions,TArray<FVector2D> EndPositions,TArray<UWidget*> Widgets,UAnimatingTurnOrderBox* Owner);
	virtual void OnStart() override;
};
UCLASS()
class TACTICALROGUELITE_API UAnimatingTurnOrderBox : public UVerticalBox
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<UUserWidget*> ActiveWidgets;
	FVector2D WidgetOffset;
public:
	void UpdateOrder(TArray<int> NewOrder);
	FVector2D GetWidgetCenterLocation(UWidget * Widget);
	void InitializeSpacing(TSubclassOf<UUserWidget> Prefab);
};
