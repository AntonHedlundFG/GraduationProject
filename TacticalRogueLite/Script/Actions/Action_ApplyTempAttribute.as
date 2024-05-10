class USAction_ApplyTempAttribute: UCActionWithTimer
{
    default NumberOfTurns = 1;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        if (AffectedUnit == nullptr)
        AffectedUnit = Cast<ACUnit>(Instigator);
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        
    }

    UFUNCTION(BlueprintOverride)
    void OnTimerFinishes(ACUnit inAffectedUnit)
    {
        for(auto& mod : ModifiersAppliedToOwner)
        {
            mod.Magnitude = -mod.Magnitude;
           GetOwningComponent().ApplyAttributeChange(mod,0);
        }
    }
}