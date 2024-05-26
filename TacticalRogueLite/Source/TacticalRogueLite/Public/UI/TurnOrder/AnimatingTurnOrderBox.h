#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "Components/CanvasPanelSlot.h"
#include "AnimatingTurnOrderBox.generated.h"

class UCTurnOrderPortraitWidget;

UCLASS()
class ULerpElementsToPositions: public UExecutable
{
	GENERATED_BODY()

public:
	ULerpElementsToPositions() = default;

	void Initialize(const TArray<FVector2D>& InStartPositions, const TArray<FVector2D>& InEndPositions, const TArray<UCTurnOrderPortraitWidget*>& InWidgets,
		UCurveFloat* InLerpCurve, const float InLerpTime, const float InWaitTimeAfterAnimationFinished);

protected:
	virtual bool Execute(float DeltaTime) override;
	
	UPROPERTY()
	TArray<FVector2D> StartPositions;
	UPROPERTY()
	TArray<FVector2D> EndPositions;
	UPROPERTY()
	TArray<TObjectPtr<UCTurnOrderPortraitWidget>> Widgets;
	UPROPERTY()
	TObjectPtr<UCurveFloat> LerpCurve;
	float LerpTime;
	float WaitTimeAfterAnimationFinished = 0; 
};

UCLASS()
class TACTICALROGUELITE_API UAnimatingTurnOrderBox : public UCanvasPanel
{
	GENERATED_BODY()
	
public:
	// Returns the index at which it was added to 
	int AddWidget(UUserWidget* Widget);
	void RemoveIndex(int index);
	void RemoveWidget(UUserWidget* Widget);
	void UpdateOrder(const TArray<struct FUnitWidget>& UnitWidgetsTurnOrder, UObject* Owner);
	FVector2D GetWidgetCenterLocation(UUserWidget* Widget);
	FVector2D GetNextPosition();
	UCanvasPanelSlot* GetPanelSlot(UUserWidget* Widget);
	void InitializeSpacing(TSubclassOf<UUserWidget> Prefab);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> LerpCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeAfterAnimationFinished;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnchorData ChildAlignment;
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> ActiveWidgets;
	FVector2D WidgetOffset;
};
