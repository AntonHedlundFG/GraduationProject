class USDurationAction_Rooted : UCActionWithTimer
{

    //default NumberOfTurns = 1;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        AffectedUnit = Cast<ACUnit>(GetOwningComponent().GetOwner()); 
        UCActionComponent ActionComponent = AffectedUnit.GetActionComp();
        check(ActionComponent != nullptr);

        ActionComponent.AppendTags(ActionTags);
        
        BindTimer();
        CLogManager::Log(ELogCategory::LC_Gameplay, "Im rooted");
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        UCActionComponent ActionComponent = AffectedUnit.GetActionComp();
        check(ActionComponent != nullptr);


        ActionComponent.RemoveTags(ActionTags);
    }

    UFUNCTION(BlueprintOverride)
    void OnTimerFinishes(ACUnit inAffectedUnit)
    {
        UCActionComponent ActionComponent = AffectedUnit.GetActionComp();
        check(ActionComponent != nullptr);

        ActionComponent.RemoveTags(ActionTags);
        CLogManager::Log(ELogCategory::LC_Gameplay, "Im not rooted");
    }
}
