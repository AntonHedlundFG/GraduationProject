class USVortexPullVisualization : UCActionVisualization
{

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UndoSpeedMultiplier = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ANiagaraActor> VortexPullSystemClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelativeTileSize = 0.17f;
    

    // State
    USAction_PullInGridContent Action;
    float TimePassed = 0;
    TArray<ACGridContent> ContentInRange;
    TArray<ACGridTile> ContentStartTiles;
    ACGridTile TargetTile;
    ANiagaraActor VortexPullSystem;
    FVector Offset;
    float Size;

    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        TimePassed = 0;
        Action = Cast<USAction_PullInGridContent>(VisualizedAction);

        if(!IsValid(Action))
        {
            return;
        }

        TargetTile = Action.TargetTile;

        if(!IsValid(TargetTile))
        {
            return;
        }

        ContentInRange = Action.ContentInRange;
        ContentStartTiles = Action.ContentStartTiles;

        /* This should not be required since the effect can play without targets.
        if(ContentInRange.Num() == 0)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "No content in range for VortexPullVisualization");
            return;
        }*/

        if(ContentInRange.Num() != ContentStartTiles.Num())
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "ContentInRange and ContentToStartTileMap have different sizes for VortexPullVisualization");
            return;
        }

        Size = Action.Range * 2 * RelativeTileSize + RelativeTileSize;

        // Spawn niagara system actor
        VortexPullSystem = Cast<ANiagaraActor>(SpawnActor(VortexPullSystemClass, TargetTile.GetActorLocation(), FRotator::ZeroRotator));

        if(IsValid(VortexPullSystem))
        {
            VortexPullSystem.SetActorScale3D(FVector(1, 1, 1));
            // Set user variables
            VortexPullSystem.NiagaraComponent.SetFloatParameter(FName("Duration"), Duration);
            VortexPullSystem.NiagaraComponent.SetFloatParameter(FName("Size"), Size); 
        }
    }

    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        TimePassed = Math::Clamp(TimePassed + DeltaTime, 0, Duration);

        if(!IsValid(Action))
        {
            return false;
        }

        for (int i = 0; i < ContentInRange.Num(); i++)
        {
            ACGridContent Content = ContentInRange[i];
            if(IsValid(Content))
            {
                ACGridTile StartTile = ContentStartTiles[i];

                if(IsValid(StartTile))
                {
                    FVector StartLocation = StartTile.GetActorLocation() + Offset;
                    FVector EndLocation = Content.GetTile().GetActorLocation() + Offset;
                    float Alpha = TimePassed / Duration;

                    FVector NewLocation = MathExtended::EaseInOutBackVector(StartLocation, EndLocation, Alpha);

                    Content.SetActorLocation(NewLocation);
                }
            }
        }

        if( DeltaTime >= 0 ? TimePassed >= Duration : TimePassed <= 0 ){

            // Set start tile to new tile for Undo
            for (int i = 0; i < ContentInRange.Num(); i++)
            {
                ContentStartTiles[i] = ContentInRange[i].GetTile();
            }
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

}