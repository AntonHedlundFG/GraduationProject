class UArmorAction_Visualization : UCActionVisualization
{
    UPROPERTY(NotVisible)
    USAction_ApplyTempAttribute ActionClass;

     UPROPERTY()
    TSubclassOf<AActor> VisualEffectActor;

    //Ref to instantiated vfx actor.
    AActor SpawnedVFXActor;

    UFUNCTION(BlueprintOverride)
    bool CanVisualizeAction(UCAction Action)
    {
        return Action.IsA(USAction_ApplyTempAttribute::StaticClass());
    }
    
      UFUNCTION(BlueprintOverride)
    void Enter()
    {
         if (!VisualEffectActor.IsValid()) return;
        ActionClass = Cast<USAction_ApplyTempAttribute>(VisualizedAction);

        if (!IsValid(ActionClass) || !IsValid(ActionClass.AffectedUnit))
        {
            return;
        }

        ActionClass.GetOwningComponent().OnActionStopped.AddUFunction(this, n"OnArmorFinish");
        ActionClass.GetOwningComponent().OnActionUndo.AddUFunction(this, n"OnArmorFinish");

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
    private void OnArmorFinish(UCActionComponent ActionComponent, UCAction EffectAction)
    {

        if (EffectAction != ActionClass)
        {
            return;
        }
      
        SpawnedVFXActor.DestroyActor();
        ActionClass.GetOwningComponent().OnActionStopped.Unbind(this, n"OnArmorFinish");
        ActionClass.GetOwningComponent().OnActionUndo.Unbind(this, n"OnArmorFinish");
    }

}