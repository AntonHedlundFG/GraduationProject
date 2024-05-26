#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "CTurnOrderPortraitWidget.h"
#include "Blueprint/UserWidget.h"
#include "Utility/CoroutineSystem/CCORExecutor.h"
#include "CTurnOrderUI.generated.h"

class UAnimatingTurnOrderBox;
class ACUnit;

USTRUCT()
struct FUnitWidget
{
    GENERATED_BODY()

    FUnitWidget() = default;

    FUnitWidget(const TObjectPtr<ACUnit> inUnit, const TObjectPtr<UCTurnOrderPortraitWidget> inWidget, const ESlateVisibility inVisibility,
                const FString& inText, const FSlateBrush& inBackgroundBrush)
        : Unit(inUnit), Widget(inWidget), Visibility(inVisibility), Text(inText), BackgroundBrush(inBackgroundBrush) {}

    UPROPERTY()
    TObjectPtr<ACUnit> Unit;

    UPROPERTY()
    TObjectPtr<UCTurnOrderPortraitWidget> Widget;

    UPROPERTY()
    ESlateVisibility Visibility;

    UPROPERTY()
    FString Text;

    UPROPERTY()
    FSlateBrush BackgroundBrush;

    bool operator==(const ACUnit* OtherUnit) const
    {
        return Unit == OtherUnit;
    }
};

UCLASS()
class TACTICALROGUELITE_API UCTurnOrderUI : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    void FetchNewTurnOrder(TArray<ACUnit*>& NewTurnOrder) const;
    UFUNCTION()
    void UpdateTurnList();
    bool ValidateUnitWidgetMap(const TArray<ACUnit*>& TurnOrder);
    void UpdateUnitWidgetMap(const TArray<ACUnit*>& TurnOrder);
    void HandleRemoval(ACUnit* Unit);
    void HandleAddition(ACUnit* Unit);

    UPROPERTY()
    TArray<FUnitWidget> UnitWidgets;

    UPROPERTY()
    ACGameState* GameState;

    UPROPERTY()
    UCCORExecutor* Executor;

    UPROPERTY(meta = (BindWidget))
    UAnimatingTurnOrderBox* TurnOrderBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
    TSubclassOf<UCTurnOrderPortraitWidget> PortraitWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
    FSlateBrush EnemyBackgroundBrush;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
    FSlateBrush PlayerBackgroundBrush;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
    float AnimationWaitTime = 0.15f;
};

UCLASS()
class TACTICALROGUELITE_API UAnimateOutPortraitWidgets_Executable : public UExecutable
{
    GENERATED_BODY()

public:
    UAnimateOutPortraitWidgets_Executable() = default;

    void Initialize(const TObjectPtr<UCTurnOrderPortraitWidget> inPortrait, UCTurnOrderUI* inTurnOrderUI,
                    UAnimatingTurnOrderBox* inTurnOrderBox, float inWaitTimeAfterCompletion);

    virtual bool Execute(float DeltaTime) override;
    virtual void Start() override;
    virtual void End() override;

private:
    UPROPERTY()
    TObjectPtr<UCTurnOrderPortraitWidget> Portrait;

    UPROPERTY()
    TObjectPtr<UCTurnOrderUI> TurnOrderUI;

    UPROPERTY()
    TObjectPtr<UAnimatingTurnOrderBox> TurnOrderBox;

    float WaitTimeAfterCompletion;
};

UCLASS()
class TACTICALROGUELITE_API UAnimationInPortraitWidget_Executable : public UExecutable
{
    GENERATED_BODY()

public:
    UAnimationInPortraitWidget_Executable() = default;

    void Initialize(const TObjectPtr<UCTurnOrderPortraitWidget> inPortrait, float inWaitTimeAfterCompletion);

    virtual bool Execute(float DeltaTime) override;
    virtual void Start() override;
    virtual void End() override;

private:
    UPROPERTY()
    TObjectPtr<UCTurnOrderPortraitWidget> Portrait;

    float WaitTimeAfterCompletion;
};