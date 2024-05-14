class USDurationAction_Thorns : UCActionWithTimer
{
    UPROPERTY(Replicated)
    int DamageAmount = 1;

    default NumberOfTurns = 1;

    UPROPERTY(Replicated)
    bool bCanThornsAtRange = false;

    FAttributeChangedSignature Signature;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        
        AffectedUnit = Cast<ACUnit>(Instigator);
        
        //Enable thorns
        //AffectedUnit.AttributeComp.OnHealthChanged.AddUFunction(this, n"OnHealthChanged");
        
        Signature.BindUFunction(this, n"OnHealthChanged");
        
        AffectedUnit.ActionComp.AddAttributeChangedListener(GameplayTags::Attribute_Health, Signature, false);
    
        CLogManager::Log(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} gained thorns.");
    }

    UFUNCTION()
    private void OnHealthChanged(UCActionComponent ActionComponent,
                                         UCActionComponent InstigatorComponent, FGameplayTag AttributeTag,
                                         int NewValue, int Delta,
                                         const FGameplayTagContainer&in ContextTags,
                                         EAttributeModifierOperation ModType)
    {
        if (!IsValid(ActionComponent) || !IsValid(InstigatorComponent)) return;
        ACUnit Attacker = Cast<ACUnit>(InstigatorComponent.GetOwner());
        if (Attacker == nullptr) return;
        ACUnit Defender = Cast<ACUnit>(ActionComponent.GetOwner());
        if (Defender == nullptr) return;

        if (Delta >= 0 || ContextTags.HasTag(GameplayTags::Action_Thorns))
            return;
    
        //If we can only thorns in melee, make sure we are in melee range of attacker.
        auto NeighbourTiles = Defender.GetTile().GetNeighbours(false);
        auto AttackerTile = Attacker.GetTile();
        if (!bCanThornsAtRange && !NeighbourTiles.Contains(AttackerTile)) return;

        ACGameMode GameMode = Cast<ACGameMode>(Gameplay::GetGameMode());
        if (GameMode == nullptr) return;


        //We generate an instance of damage here so that we can undo it without interfering with the thorns effect itself.
        USThornsDamageTriggeredAction ThornsAction = Cast<USThornsDamageTriggeredAction>(NewObject(Outer, USThornsDamageTriggeredAction::StaticClass()));
        ThornsAction.DamageAmount = DamageAmount;
        ThornsAction.TargetUnit = Attacker;
        ThornsAction.ThornsSource = Defender;
        GameMode.RegisterAction(ThornsAction);

    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        AffectedUnit.ActionComp.RemoveAttributeChangedListener(GameplayTags::Attribute_Health, Signature);

        CLogManager::Log(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} lost thorns.");
    }

    UFUNCTION(BlueprintOverride)
    void OnTimerFinishes(ACUnit inAffectedUnit)
    {
        AffectedUnit.ActionComp.RemoveAttributeChangedListener(GameplayTags::Attribute_Health, Signature);

        CLogManager::Log(ELogCategory::LC_Gameplay, f"{AffectedUnit.UnitName} no longer has thorns.");
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

    UPROPERTY()
    int ActualDamage;

    UPROPERTY()
    FAttributeModifications UndoMods;


    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        FGameplayTagContainer ContextTags;
        ContextTags.AddTag(GameplayTags::Action_Thorns);
        UndoMods = CGameplay::ApplyDamage(ThornsSource, TargetUnit, DamageAmount, ContextTags);

        CLogManager::Log(ELogCategory::LC_Gameplay, f"{TargetUnit.UnitName} took {DamageAmount} thorns damage from {ThornsSource.UnitName}.");
    }
    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        FGameplayTagContainer ContextTags;

        for (FAttributeModification& Mod : UndoMods.Modifications)
            TargetUnit.ActionComp.ApplyAttributeChange(Mod, 0);

        CLogManager::Log(ELogCategory::LC_Gameplay, f"{TargetUnit.UnitName} undid taking {DamageAmount} thorns damage from {ThornsSource.UnitName}.");
    }
    
}

UCLASS(Abstract)
class USThornsDamageVisualization : UCActionVisualization
{
    UPROPERTY(NotVisible)
    USThornsDamageTriggeredAction ThornsAction;

    //The actor spawned by this visualization. It will be spawned ON the character with thorns, and its Forward direction will be pointing towards the attacker.
    UPROPERTY()
    TSubclassOf<AActor> VisualEffectActor;

    //If set to more than 0.0f, the spawned actor will have this lifespan. Otherwise, you have to manage lifespan separately.
    UPROPERTY()
    float VisualEffectLifespan = 0.0f;
    
    UFUNCTION(BlueprintOverride)
    bool CanVisualizeAction(UCAction Action)
    {
        return Action.IsA(USThornsDamageTriggeredAction::StaticClass());
    }
    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        if (!VisualEffectActor.IsValid()) return;
        ThornsAction = Cast<USThornsDamageTriggeredAction>(VisualizedAction);
        if (!IsValid(ThornsAction) || !IsValid(ThornsAction.ThornsSource) || !IsValid(ThornsAction.TargetUnit)) return;
        
        FVector Forward = ThornsAction.TargetUnit.GetActorLocation() - ThornsAction.ThornsSource.GetActorLocation();
        Forward.Normalize();
        FVector Up = FVector::UpVector;
        FVector Right = Forward.CrossProduct(Up);
        Up = Right.CrossProduct(Forward); //Shouldn't be necessary since the units should be on the same XY plane, but I do this anyway for safety.
        FRotator Rotation = FRotator::MakeFromAxes(Forward, Right, Up);
        
        AActor SpawnedActor = SpawnActor(VisualEffectActor, ThornsAction.ThornsSource.GetActorLocation(), Rotation);
        if (VisualEffectLifespan > 0.0f)
            SpawnedActor.SetLifeSpan(VisualEffectLifespan);
    }
    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        return true;   
    }
    UFUNCTION(BlueprintOverride)
    bool RevertTick(float DeltaTime)
    {
        return true;
    }

}