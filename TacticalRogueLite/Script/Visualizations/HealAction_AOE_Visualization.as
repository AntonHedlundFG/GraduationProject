class USHealAction_AOE_Visualization : UCActionVisualization
{
    UPROPERTY()
    private float timer;
    UPROPERTY(EditAnywhere)
    float TotalLifeSpan;
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> AOEEffect;
    UPROPERTY(EditAnywhere)
    float AOEPropagationInSeconds;
    UPROPERTY(EditAnywhere)
    float AOEEstimatedSizeInUnits;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> OnHitEffect;

    TSet<ACUnit> UnitsAlreadyProcessed;

    USHealAction_AOE HealAction;

    TArray<ACUnit> Targets;
    ACUnit Instigator;

    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        HealAction = Cast<USHealAction_AOE>(VisualizedAction);
        Targets = HealAction.TargetsArray;
        Instigator = HealAction.AttackingUnit;
        AActor AoeEffectActor = SpawnActor(AOEEffect,Instigator.GetActorLocation(),FRotator::ZeroRotator);
        UnitsAlreadyProcessed.Empty();
        timer = 0;
    }
    UFUNCTION(BlueprintOverride)
    void Exit()
    {
        
    }
    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        for(auto Target : Targets)
        {
            if(ShouldSpawnOnHitEffect(timer/AOEPropagationInSeconds,Instigator,Target))
            {
                AActor OnHitEffectActor = SpawnActor(OnHitEffect,Target.GetActorLocation(),FRotator::ZeroRotator);
                OnHitEffectActor.SetLifeSpan(TotalLifeSpan);
            }
        }
        if(timer >= AOEPropagationInSeconds)
        {
            return true;
        }

        timer += DeltaTime;
        return false;
    }
    UFUNCTION(BlueprintOverride)
    bool RevertTick(float DeltaTime)
    {
        return true;
    }
    bool ShouldSpawnOnHitEffect(float T, ACUnit Origin,ACUnit Target)
    {
        if(UnitsAlreadyProcessed.Contains(Target)){return false;}

        FVector aPos = Origin.GetActorLocation();
        aPos.Z = 0;
        FVector bPos = Target.GetActorLocation();
        bPos.Z = 0;
        float DeltaDistance = (aPos-bPos).Size()/AOEEstimatedSizeInUnits;
        if(DeltaDistance <= T)
        {
            UnitsAlreadyProcessed.Add(Target);
            return true;
        }
        return false;
    }
}