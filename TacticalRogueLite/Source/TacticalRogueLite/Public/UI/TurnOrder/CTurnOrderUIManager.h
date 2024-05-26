#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "CTurnOrderUIManager.generated.h"

#pragma region AnimationTasks

UCLASS()
class UTurnOrderAnimationTask : public UExecutable
{
	GENERATED_BODY()
protected:
	float WaitTimeAfterCompletion = 0;
	float WaitTimeBetweenAnimations = 0;
	UPROPERTY()
	TArray<TObjectPtr<UCTurnOrderPortraitWidget>> Portraits;
};

UCLASS()
class UTurnOrderAnimationTask_Remove : public UTurnOrderAnimationTask
{
	GENERATED_BODY()
public:
	UTurnOrderAnimationTask_Remove() = default;

	void Initialize(const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits, const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations);
	virtual bool Execute(float DeltaTime) override;
};

UCLASS()
class UTurnOrderAnimationTask_Add : public UTurnOrderAnimationTask
{
	GENERATED_BODY()
public:
	UTurnOrderAnimationTask_Add() = default;
	
	void Initialize( const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits,
			const TArray<FVector2D>& inPositions, const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations);
	virtual bool Execute(float DeltaTime) override;

protected:
	TArray<FVector2D> Positions;
};

UCLASS()
class UTurnOrderAnimationTask_MoveTo : public UTurnOrderAnimationTask
{
	GENERATED_BODY()
public:
	UTurnOrderAnimationTask_MoveTo() = default;
	
	void Initialize( const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits, const TArray<FVector2D>& inPositions, const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations, UCurveFloat* inAnimationEasing);
	virtual bool Execute(float DeltaTime) override;

protected:
	UPROPERTY()
	TArray<FVector2D> Positions;
	UPROPERTY()
	TObjectPtr<UCurveFloat> AnimationEasing;
	UPROPERTY()
	TArray<FVector2D> StartPositions;
};

UCLASS()
class UTurnOrderAnimationTask_EnqueueWidgets : public UTurnOrderAnimationTask
{
	GENERATED_BODY()
public:
	UTurnOrderAnimationTask_EnqueueWidgets() = default;

	void Initialize(const TArray<UCTurnOrderPortraitWidget*>& inWidgetsToEnqueue, ACTurnOrderUIManager* inTurnManager);
	virtual bool Execute(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<ACTurnOrderUIManager> TurnManager;
	UPROPERTY()
	TArray<TObjectPtr<UCTurnOrderPortraitWidget>> WidgetsToEnqueue;
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
	UFUNCTION()
	void UpdateTurnList();
	UPROPERTY()
	TArray<ACUnit*> LastTurnOrder;
	UPROPERTY()
	TMap<ACUnit*,UCTurnOrderPortraitWidget*> ActivePortraits;
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
