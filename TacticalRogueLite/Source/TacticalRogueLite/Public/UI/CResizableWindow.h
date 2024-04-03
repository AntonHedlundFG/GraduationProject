// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMovableWindow.h"
#include "CResizableWindow.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EResizeType : uint8
{
	None            UMETA(Hidden),
	Diagonal		UMETA(DisplayName = "Diagonal"),
	Horizontal		UMETA(DisplayName = "Horizontal"),
	Vertical		UMETA(DisplayName = "Vertical"),
};
class UButton;
UCLASS()
class TACTICALROGUELITE_API UCResizableWindow : public UCMovableWindow
{
	GENERATED_BODY()


	FTimerHandle resizeTimerHandle;
	EResizeType ScalingType = None;
	FVector2D ClickDifference = FVector2D::Zero();
	UFUNCTION()
	void ClearResizeTimer();
	void SetClickDifference();
	FVector2D GetNewCanvasScale();

	public:
	
	UFUNCTION()
	void UpdateScale();
	void StartResizeTimer();
	UFUNCTION()
	void OnDiagonalPressed();
	UFUNCTION()
	void OnHorizontalPressed();
	UFUNCTION()
	void OnVerticalPressed();
	virtual void SetUp() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* DiagonalButton;
	UPROPERTY(meta=(BindWidget))
	UButton* HorizontalButton;
	UPROPERTY(meta=(BindWidget))
	UButton* VerticalButton;
	


};
