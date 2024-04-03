// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "MeshAttributes.h"
#include "UObject/NoExportTypes.h"
#include "CTurnOrderUIManager.generated.h"

/**
 * 
 */
#pragma region AnimationTasks
USTRUCT()
struct FTurnOrderAnimationTask
{
	GENERATED_BODY()
	float WaitTimeAfterCompletion = 0;
	float WaitTimeBetweenAnimations = 0;
	TArray<UCTurnOrderPortraitWidget*> Portraits;
	virtual bool Execute(){return true;}
	virtual ~FTurnOrderAnimationTask();
};

struct FTurnOrderAnimationTask_Remove : FTurnOrderAnimationTask
{
public:
	FTurnOrderAnimationTask_Remove(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations);
	//virtual~FTurnOrderAnimationTask_Remove()override;
	virtual bool Execute() override;
};
struct FTurnOrderAnimationTask_Add : FTurnOrderAnimationTask
{
	TArray<FVector2D> Positions;
public:
	FTurnOrderAnimationTask_Add(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits,TArray<FVector2D> Positions, float WaitTimeAfterCompletion,float WaitTimeBetweenAnimations);
	//virtual~FTurnOrderAnimationTask_Add()override;
	virtual bool Execute() override;
};

struct FTurnOrderAnimationTask_MoveTo : FTurnOrderAnimationTask
{
	TArray<FVector2D> Positions;
	public:
	FTurnOrderAnimationTask_MoveTo(TArray<UCTurnOrderPortraitWidget*> AffectedPortraits, TArray<FVector2D> Positions, float WaitTimeAfterCompletion, float WaitTimeBetweenAnimations);
	//virtual~FTurnOrderAnimationTask_MoveTo()override;
	virtual bool Execute() override;
};
#pragma endregion 
UCLASS()
class TACTICALROGUELITE_API ACTurnOrderUIManager : public AActor
{
	
	TQueue<UCTurnOrderPortraitWidget*> WidgetPool;
	UPROPERTY()
	ACGameState* GameState;
	GENERATED_BODY()
	virtual void BeginPlay() override;
	UFUNCTION()
	void UpdateTurnList();
	TArray<ACUnit*> LastTurnOrder;

	//PoolingSystem will be refactored later on
	UCTurnOrderPortraitWidget* CreatePortraitWidget();
	UCTurnOrderPortraitWidget* DeQueuePortraitWidget();
	void EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget);
	void HandleEnqueue(UCTurnOrderPortraitWidget* widget);
	void HandleDequeue(UCTurnOrderPortraitWidget* widget);
	TArray<FVector2D> CalculateVieportPositionOfPortrait(int AmountOfUnits);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	
};
