class USChainLightningVisualization : UCActionVisualization
{

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DurationPerBounce = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UndoSpeedMultiplier = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNiagaraSystem ChainLightningSystem;  
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Width = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector HitOffset = FVector(0, 0, 0);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LightningStrikeStartHeight = 500.0f;

    // State
    float TotalDuration;
    UCAttackAction_ChainLightning Action;
    float TimePassed = 0;
    TArray<ACUnit> Targets;

    ACUnit Source;
    int Index = 0;

    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        Action = Cast<UCAttackAction_ChainLightning>(VisualizedAction);
        Targets = Action.UnitsHit;
        TotalDuration = DurationPerBounce * Targets.Num();

        Source = Action.AttackingUnit;
        SpawnChainLightning();

    }

    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        TimePassed = Math::Clamp(TimePassed + DeltaTime, 0, TotalDuration);
     
        if( DeltaTime >= 0 ? TimePassed >= TotalDuration : TimePassed <= 0 ){
            TimePassed = 0;
            return true;
        }

        return false;
    }

    UFUNCTION(BlueprintOverride)
    bool RevertTick(float DeltaTime)
    {
        return Tick(DeltaTime * UndoSpeedMultiplier);
    }

    UFUNCTION()
    void SpawnChainLightning()
    {
        ACUnit Target = Targets[Index];
        if( !IsValid(Target) || !IsValid(Source) )
        {
            return;
        }

        // Spawn the chain lightning
        UNiagaraComponent System = Niagara::SpawnSystemAtLocation(ChainLightningSystem, Source.GetActorLocation());
        System.SetVectorParameter(FName("Start"), Source.GetActorLocation() + HitOffset);
        System.SetVectorParameter(FName("End"), Target.GetActorLocation() + HitOffset);
        System.SetVectorParameter(FName("ImpactNormal"), (Target.GetActorLocation() - Source.GetActorLocation()).GetSafeNormal());
        System.SetFloatParameter(FName("Width"), Width);

        // Spawn the lightning strike
        System = Niagara::SpawnSystemAtLocation(ChainLightningSystem, Target.GetActorLocation());
        System.SetVectorParameter(FName("Start"), Target.GetActorLocation() + HitOffset);
        System.SetVectorParameter(FName("End"), Target.GetActorLocation() + FVector::UpVector * LightningStrikeStartHeight);
        System.SetVectorParameter(FName("ImpactNormal"), FVector::UpVector);
        System.SetFloatParameter(FName("Width"), Width);


        Index++;

        if( Index >= Targets.Num() )
        {
            return;
        }

        Source = Target;

        System::SetTimer(this, n"SpawnChainLightning", DurationPerBounce, false);
    }

}