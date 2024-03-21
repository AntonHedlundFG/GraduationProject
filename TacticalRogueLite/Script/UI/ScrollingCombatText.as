enum ECombatTextAnimationType
{
    Rise,
    Fall,
    Fountain
}


class USScrollingCombatTextComponent : UActorComponent
{
    TSubclassOf<AActor> TextObject;
    ECombatTextAnimationType AnimationType = ECombatTextAnimationType::Rise;

    USObjectPool TextPool;
    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        TextPool = UObjectPoolSubsystem::Get().GetPool(TextObject);   
    }

    UFUNCTION(BlueprintCallable)
    void SpawnCombatText()
    {

    }
}

class ASScrollingCombatTextActor :ASPoolableActor
{
    UPROPERTY(DefaultComponent,BlueprintReadWrite)
    UWidgetComponent TextWidget;
    void OnEnQueue() override
    {
        Super::OnEnQueue();
    }
    void OnDequeue() override
    {
        Super::OnDequeue();
        
    }
    UFUNCTION(BlueprintEvent,Category = events)
    void TriggerAnimation(){}
}