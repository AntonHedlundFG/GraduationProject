class USHellFireVisualization : UCActionVisualization
{

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ANiagaraActor> BlastNiagaraSystemClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelativeTileSize = 0.17f;


    // State
    USAction_AoEDamage Action;
    float TimePassed = 0;
    ACGridTile TargetTile;
    FVector Offset;
    float Size;

    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        TimePassed = 0;
        Action = Cast<USAction_AoEDamage>(VisualizedAction);

        if(!IsValid(Action))
        {
            return;
        }

        TargetTile = Action.StartTile;

        if(!IsValid(TargetTile))
        {
            return;
        }

        Offset = FVector::ZeroVector;
        Offset.Z = Action.AttackingUnit.GetActorLocation().Z - Action.AttackingUnit.GetTile().GetActorLocation().Z;

        Size = Action.Range * 2 * RelativeTileSize + RelativeTileSize;

        // Spawn niagara system actor
        ANiagaraActor InstancedNiagaraSystem = Cast<ANiagaraActor>(SpawnActor(BlastNiagaraSystemClass, TargetTile.GetActorLocation() + Offset, FRotator::ZeroRotator));

        if(IsValid(InstancedNiagaraSystem))
        {
            InstancedNiagaraSystem.SetActorScale3D(FVector(1, 1, 1));
            // Set user variables
            InstancedNiagaraSystem.NiagaraComponent.SetFloatParameter(FName("Size"), Size); 
        }
    }

    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        TimePassed = Math::Clamp(TimePassed + DeltaTime, 0, Duration);

        if( TimePassed >= Duration ){
            TimePassed = 0;
            return true;
        }

        return false;
    }

    UFUNCTION(BlueprintOverride)
    bool RevertTick(float DeltaTime)
    {
        return true;
    }

}