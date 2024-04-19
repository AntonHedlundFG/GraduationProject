// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTurnOrderPortraitWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "Components/CanvasPanelSlot.h"
#include "AnimatingTurnOrderBox.generated.h"

/**
 * 
 */
struct FLerpElementsToPositions: public FExecutable
{
	TArray<FVector2D> StartPositions;
	TArray<FVector2D> EndPositions;
	TArray<UCTurnOrderPortraitWidget*> Widgets;
	UCurveFloat* LerpCurve;
	float LerpTime;
	float WaitTimeAfterAnimationFinished = 0; 
	FLerpElementsToPositions(TArray<FVector2D> StartPositions,TArray<FVector2D> EndPositions,TArray<UCTurnOrderPortraitWidget*> Widgets,UCurveFloat* LerpCurve,float LerpTime,float WaitTimeAfterAnimationFinished);
	virtual bool Execute(float DeltaTime) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
};
UCLASS()
class TACTICALROGUELITE_API UAnimatingTurnOrderBox : public UCanvasPanel
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<UUserWidget*> ActiveWidgets;
	FVector2D WidgetOffset;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* LerpCurve;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LerpTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float WaitTimeAfterAnimationFinished;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAnchorData ChildAlignment;
	//Returns the index at which it was added to 
	int AddWidget(UUserWidget* Widget);
	void RemoveIndex(int index);
	void UpdateOrder(TArray<int> NewOrder,UObject* Owner);
	FVector2D GetWidgetCenterLocation(UUserWidget * Widget);
	FVector2D GetNextPosition();
	UCanvasPanelSlot* GetPanelSlot(UUserWidget* Widget);
	void InitializeSpacing(TSubclassOf<UUserWidget> Prefab);
};
