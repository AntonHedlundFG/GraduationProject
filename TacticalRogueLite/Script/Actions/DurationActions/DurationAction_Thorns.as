class USDurationAction_Thorns : UCActionWithTimer
{
    UPROPERTY(Replicated)
    int DamageAmount = 1;

    default NumberOfTurns = 1;

    UPROPERTY(Replicated)
    bool bCanThornsAtRange = false;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        if (AffectedUnit == nullptr)
            AffectedUnit = Cast<ACUnit>(Instigator);
        
        //Enable thorns
        AffectedUnit.AttributeComp.OnHealthChanged.AddUFunction(this, n"OnHealthChanged");
        
        BindTimer();
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} gained thorns.");
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        //Disable thorns
        AffectedUnit.AttributeComp.OnHealthChanged.Unbind(this, n"OnHealthChanged");
        
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} lost thorns.");
    }

    UFUNCTION(BlueprintOverride)
    void OnTimerFinishes(ACUnit inAffectedUnit)
    {
        //Disable thorns
        AffectedUnit.AttributeComp.OnHealthChanged.Unbind(this, n"OnHealthChanged");

        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} no longer has thorns.");
    }

    UFUNCTION()
    private void OnHealthChanged(AActor InstigatorActor, UCAttributeComponent OwningComp, int NewValue,
                               int Delta)
    {
        ACUnit Attacker = Cast<ACUnit>(InstigatorActor);
        if (Attacker == nullptr) return;
        ACUnit Defender = Cast<ACUnit>(OwningComp.GetOwner());
        if (Defender == nullptr) return;
        
        //If we can only thorns in melee, make sure we are in melee range of attacker.
        if (!bCanThornsAtRange && !Defender.GetTile().GetNeighbours(false).Contains(Attacker.GetTile())) return;

        ACGameMode GameMode = Cast<ACGameMode>(Gameplay::GetGameMode());
        if (GameMode == nullptr) return;


        //We generate an instance of damage here so that we can undo it without interfering with the thorns effect itself.
        USThornsDamageTriggeredAction ThornsAction = Cast<USThornsDamageTriggeredAction>(NewObject(Outer, USThornsDamageTriggeredAction::StaticClass()));
        ThornsAction.DamageAmount = DamageAmount;
        ThornsAction.TargetUnit = Attacker;
        ThornsAction.ThornsSource = Defender;
        GameMode.RegisterAction(ThornsAction);
    }


}

//This class is NOT to be used anywhere else. It is simply a result of Thorns triggering. 
//If you want to grant a unit thorns, use the USGainThornsAction. It will automatically create instances of this damage class.
UCLASS(NotBlueprintable)
class USThornsDamageTriggeredAction : UCAction
{
    //This should be set by the delegate that creates this object.
    UPROPERTY(Replicated)
    int DamageAmount = 1;

    //Store old health for undo functionality
    UPROPERTY()
    int OldHealth;

    //The attacker that deals attack damage and should be thorns'd
    UPROPERTY(Replicated)
    ACUnit TargetUnit;

    //The unit that has the thorns buff
    UPROPERTY(Replicated)
    ACUnit ThornsSource;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        UCAttributeComponent Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
        OldHealth = Attributes.GetHealth();

        CGameplay::ApplyDamage(ThornsSource, TargetUnit, DamageAmount);

        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{TargetUnit.UnitName} took {DamageAmount} thorns damage from {ThornsSource.UnitName}.");
    }
    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        UCAttributeComponent Attributes = UCAttributeComponent::GetAttributes(TargetUnit);
        Attributes.SetHealth(OldHealth);

        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{TargetUnit.UnitName} undid taking {DamageAmount} thorns damage from {ThornsSource.UnitName}.");
    }
}

