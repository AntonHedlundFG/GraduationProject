class URootAction_Visualization : UCActionVisualization
{
    UPROPERTY(NotVisible)
    USDurationAction_Rooted ActionClass;

     UPROPERTY()
    TSubclassOf<AActor> VisualEffectActor;

    //Ref to instantiated vfx actor.
    AActor SpawnedVFXActor;

    UFUNCTION(BlueprintOverride)
    bool CanVisualizeAction(UCAction Action)
    {
        return Action.IsA(USDurationAction_Rooted::StaticClass());
    }
    
      UFUNCTION(BlueprintOverride)
    void Enter()
    {
         if (!VisualEffectActor.IsValid()) return;
        ActionClass = Cast<USDurationAction_Rooted>(VisualizedAction);

        if (!IsValid(ActionClass) || !IsValid(ActionClass.AffectedUnit))
        {
            return;
        }

        ActionClass.GetOwningComponent().OnActionStopped.AddUFunction(this, n"OnRootFinish");
        ActionClass.GetOwningComponent().OnActionUndo.AddUFunction(this, n"OnRootFinish");

        FVector Location = ActionClass.AffectedUnit.GetActorLocation();
        Location.Z += 75;
    
        SpawnedVFXActor = SpawnActor(VisualEffectActor, Location, FRotator::ZeroRotator);
        SpawnedVFXActor.SetOwner(Cast<AActor>(ActionClass.AffectedUnit));

    }

     UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        return true;
    }

    
    UFUNCTION()
    private void OnRootFinish(UCActionComponent ActionComponent, UCAction EffectAction)
    {

        if (EffectAction != ActionClass)
        {
            return;
        }
      
        SpawnedVFXActor.DestroyActor();
        ActionClass.GetOwningComponent().OnActionStopped.Unbind(this, n"OnRootFinish");
        ActionClass.GetOwningComponent().OnActionUndo.Unbind(this, n"OnRootFinish");
    }

}