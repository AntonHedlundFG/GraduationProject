// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Sound/SoundCue.h"
#include "CMovableWindow.generated.h"

class UCanvasPanelSlot;
class UButton;
class UCanvasPanel;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChange);
UCLASS()
class TACTICALROGUELITE_API UCMovableWindow : public UUserWidget
{
	GENERATED_BODY()
	FTimerHandle moveTimerHandle;
	FVector2D ClickDifference_Movable = FVector2D::Zero();
	UFUNCTION()
	void ClearMoveTimer();

protected:
	FVector2D ClampWindowSize(FVector2D vector);
	FVector2D GetMousePosition();
	UCanvasPanelSlot* GetCanvasPanelSlot();
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsOpen = false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D WindowMinSize = FVector2D(200,200);
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D WindowMaxSize;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetUp();

	UFUNCTION()
	void UpdatePosition();
	void StartMoveTimer();

	UFUNCTION()
	void OnHeaderButtonPressed();

	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION(BlueprintCallable)
	void Open();
	UPROPERTY(meta=(BindWidget))
	UButton* HeaderButton;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanelWrapper;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(BlueprintAssignable)
	FOnChange OnWindowClose;
	UPROPERTY(BlueprintAssignable)
	FOnChange OnWindowOpen;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundCue* CloseSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundCue* OpenSound;
	
};
