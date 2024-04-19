// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimatingTurnOrderBox.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "Blueprint/UserWidget.h"
#include "CTurnOrderUI.generated.h"
class ACUnit;
/**
 * 
 */


UCLASS()
class TACTICALROGUELITE_API UCTurnOrderUI : public UUserWidget
{
	GENERATED_BODY()
	TQueue<UCTurnOrderPortraitWidget*> WidgetPool;
	UPROPERTY()
	TMap<ACUnit*,UCTurnOrderPortraitWidget*> ActivePortraits;
	UPROPERTY()
	ACGameState* GameState;
	UPROPERTY()
	UCCORExecutor* Executor;
	UPROPERTY()
	TArray<ACUnit*> LastTurnOrder;
	UPROPERTY()
	TMap<ACUnit*,int> LastTurnOrder_UnitToIndex;
	UCTurnOrderPortraitWidget* CreatePortraitWidget();
	UCTurnOrderPortraitWidget* DeQueuePortraitWidget();
	void HandleEnqueue(UCTurnOrderPortraitWidget* widget);
	void HandleDequeue(UCTurnOrderPortraitWidget* widget);
	UCTurnOrderPortraitWidget* GetActiveWidget(ACUnit* key);
	UFUNCTION()
	void UpdateTurnList();
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UAnimatingTurnOrderBox* TurnOrderBox;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Defaults)
	TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Defaults)
	int AnimationWaitTime = 0.15f;
	void EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget);
};
struct FAnimateOutPortraitWidgets_Executable : public FExecutable
{
	TArray<UCTurnOrderPortraitWidget*> Portraits;
	UCTurnOrderUI* TurnOrderUI;
	UAnimatingTurnOrderBox* TurnOrderBox;
	float WaitTimeAfterCompletion;
	FAnimateOutPortraitWidgets_Executable(TArray<UCTurnOrderPortraitWidget*> Portraits,UCTurnOrderUI* TurnOrderUI,UAnimatingTurnOrderBox* TurnOrderBox,float WaitTimeAfterCompletion);
	virtual bool Execute(float DeltaTime) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
};
struct FAnimationInPortraitWidget_Executable: public FExecutable
{
	TArray<UCTurnOrderPortraitWidget*> Portraits;
	float WaitTimeAfterCompletion;
	FAnimationInPortraitWidget_Executable(TArray<UCTurnOrderPortraitWidget*> Portraits,float WaitTimeAfterCompletion);
	virtual bool Execute(float DeltaTime) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
};