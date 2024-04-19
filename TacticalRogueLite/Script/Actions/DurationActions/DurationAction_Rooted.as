class USDurationAction_Rooted : UCActionWithTimer
{
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "Im rooting hehe");

        //GetOwningComponent().OnActionStarted.Broadcast(GetOwningComponent(), this);
    }
}