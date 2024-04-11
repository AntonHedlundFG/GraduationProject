class USThornsDamageAction : UCAction
{
    UPROPERTY(Replicated)
    int DamageAmount = 1;

    UPROPERTY()
    int OldHealth;

    UPROPERTY(Replicated)
    ACUnit TargetUnit;

    UPROPERTY(Replicated)
    ACUnit ThornsSource;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        UCAttributeComponent Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
        OldHealth = Attributes.GetHealth();
        CGameplay::ApplyDamage(ThornsSource, TargetUnit, DamageAmount);
        FString PrintString = f"{TargetUnit.UnitName} took {DamageAmount} thorns damage from {ThornsSource.UnitName}.";
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, PrintString);
    }
    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        UCAttributeComponent Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
        Attributes.SetHealth(OldHealth);
        FString PrintString = f"{TargetUnit.UnitName} undid taking {DamageAmount} thorns damage from {ThornsSource.UnitName}.";
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, PrintString);
    }
}

class USGainThornsAction : UCActionWithTimer
{
    UPROPERTY(Replicated)
    int DamageAmount = 1;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        AffectedUnit = Cast<ACUnit>(Instigator);
        AffectedUnit.AttributeComp.OnHealthChanged.AddUFunction(this, n"TriggerThorns");
        BindTimer();
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} gained thorns.");
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        AffectedUnit.AttributeComp.OnHealthChanged.Unbind(this, n"TriggerThorns");
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} lost thorns.");
    }

    UFUNCTION(BlueprintOverride)
    void OnTimerFinishes(ACUnit inAffectedUnit)
    {
        AffectedUnit.AttributeComp.OnHealthChanged.Unbind(this, n"TriggerThorns");
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} no longer has thorns.");
    }

    UFUNCTION()
    private void TriggerThorns(AActor InstigatorActor, UCAttributeComponent OwningComp, int NewValue,
                               int Delta)
    {
        ACUnit Attacker = Cast<ACUnit>(InstigatorActor);
        if (Attacker == nullptr) return;
        ACUnit Defender = Cast<ACUnit>(OwningComp.GetOwner());
        if (Defender == nullptr) return;
        
        if (!Defender.GetTile().GetNeighbours(false).Contains(Attacker.GetTile())) return;

        ACGameMode GameMode = Cast<ACGameMode>(Gameplay::GetGameMode());
        if (GameMode == nullptr) return;

        USThornsDamageAction ThornsAction = Cast<USThornsDamageAction>(NewObject(Outer, USThornsDamageAction::StaticClass()));
        ThornsAction.DamageAmount = DamageAmount;
        ThornsAction.TargetUnit = Attacker;
        ThornsAction.ThornsSource = Defender;
        GameMode.RegisterAction(ThornsAction);
    }


}