//DEPRECATED
enum ECombatTextAnimationType
{
    Rise,
    Fall,
    Fountain
}

//This component can be put on an actor and will spawn text objects in world space
class USScrollingCombatTextComponent : UBoxComponent
{
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> TextObject;
    USObjectPool TextPool;

    UPROPERTY(BlueprintReadWrite)
    ECombatTextAnimationType AnimationType = ECombatTextAnimationType::Rise;
    UPROPERTY(BlueprintReadWrite)
    FVector spawnAreaPositionOffset;
    UPROPERTY(BlueprintReadWrite)
    FVector spawnArea;

    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        TextPool = UObjectPoolSubsystem::Get().GetPool(TextObject);   
    }
    //Call this function to spawn a combat text. This will trigger the animation of the combat text as well and will automatically enqueue itself into it's pool.
    UFUNCTION(BlueprintCallable)
    void SpawnCombatText(FString textToDisplay)
    {
        Print("Trying to spawn CBT");
        FVector spawnPositon = spawnAreaPositionOffset + FVector(Math::RandRange(-spawnArea.X,spawnArea.X),Math::RandRange(-spawnArea.Y,spawnArea.Y),0);
        ASScrollingCombatTextActor actor = Cast<ASScrollingCombatTextActor>(TextPool.DequeueAt(true,spawnPositon,FRotator::ZeroRotator));
        Print("" +actor.Name);
        actor.TriggerAnimation(textToDisplay);
    }
}
//This is the object contains the text element and will be pooled on creation
class ASScrollingCombatTextActor : ASPoolableActor
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
    void TriggerAnimation(FString textToDisplay){}
}