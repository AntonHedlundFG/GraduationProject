// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "CTurnOrderUIManager.generated.h"

/**
 * 
 */
#pragma region AnimationTasks

USTRUCT()
struct FTurnOrderAnimationTask : public FExecutable
{
	GENERATED_BODY()
	float WaitTimeAfterCompletion = 0;
	float WaitTimeBetweenAnimations = 0;
	TArray<UCTurnOrderPortraitWidget*> Portraits;
};
struct FTurnOrderAnimationTask_Remove : FTurnOrderAnimationTask
{
public:
	FTurnOrderAnimationTask_Remove(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations);
	//virtual~FTurnOrderAnimationTask_Remove()override;
	virtual bool Execute(float DeltaTime) override;
};
struct FTurnOrderAnimationTask_Add : FTurnOrderAnimationTask
{
	TArray<FVector2D> Positions;
public:
	FTurnOrderAnimationTask_Add(TArray<UCTurnOrderPortraitWidget*>AffectedPortraits,TArray<FVector2D>Positions, float WaitTimeAfterCompletion,float WaitTimeBetweenAnimations);
	//virtual~FTurnOrderAnimationTask_Add()override;
	virtual bool Execute(float DeltaTime) override;
};

struct FTurnOrderAnimationTask_MoveTo : FTurnOrderAnimationTask
{
	TArray<FVector2D> Positions;
	UCurveFloat* AnimationEasing;
	TArray<FVector2D> StartPositions;
	public:
	FTurnOrderAnimationTask_MoveTo(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits, TArray<FVector2D> Positions, float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations,UCurveFloat* AnimationEasing);
	//virtual~FTurnOrderAnimationTask_MoveTo()override;
	virtual bool Execute(float DeltaTime) override;
};

struct FTurnOrderAnimationTask_EnqueueWidgets : FTurnOrderAnimationTask
{
	ACTurnOrderUIManager* TurnManager;
	TArray<UCTurnOrderPortraitWidget*> WidgetsToEnqueue;
	FTurnOrderAnimationTask_EnqueueWidgets(TArray<UCTurnOrderPortraitWidget*> WidgetsToEnqueue,ACTurnOrderUIManager* TurnManager);
	virtual bool Execute(float DeltaTime) override;
};
#pragma endregion 
UCLASS()
class TACTICALROGUELITE_API ACTurnOrderUIManager : public AActor
{
	ACTurnOrderUIManager();
	TQueue<UCTurnOrderPortraitWidget*> WidgetPool;
	UPROPERTY()
	ACGameState* GameState;
	GENERATED_BODY()
	virtual void BeginPlay() override;
	UPROPERTY()
	UCCORExecutor* Executor;
	//virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void UpdateTurnList();
	UPROPERTY()
	TArray<ACUnit*> LastTurnOrder;
	UPROPERTY()
	TMap<ACUnit*,UCTurnOrderPortraitWidget*> ActivePortraits;
	//TArray<FTurnOrderAnimationTask*> Tasks;
	//PoolingSystem will be refactored later on
	UCTurnOrderPortraitWidget* CreatePortraitWidget();
	UCTurnOrderPortraitWidget* DeQueuePortraitWidget();
	void HandleEnqueue(UCTurnOrderPortraitWidget* widget);
	void HandleDequeue(UCTurnOrderPortraitWidget* widget);
	UCTurnOrderPortraitWidget* GetActiveWidget(ACUnit* key);
	TArray<FVector2D> CalculateViewportPositions(int AmountOfUnits);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* AnimationMoveToEasing;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ClampMin = 0,ClampMax = 1))
	float WidgetListStartPositionOffsetFromAnchor = 0.2f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PortraitPixelOffset = 120.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AnimationTimeOffset = 0.2f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AnimationWaitTime = 0.2f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MoveAnimationLerpTime = 0.1f;
	void EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget);
};
