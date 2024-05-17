class USAction_ApplyAttributeOnLowHealth : UCAction
{
    bool bUSeAbsoluteHealthThreshold = false;
    FAttributeModification OnHealthLowModification;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        
    }
    UFUNCTION(BlueprintOverride)
    void StopAction(AActor Instigator)
    {
        
    }
    UFUNCTION()
    void OnHealthChange()
    {

    }
}