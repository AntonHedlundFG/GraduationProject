class USVortexPullVisualization : UCActionVisualization
{

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UndoSpeedMultiplier = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ANiagaraActor> VortexPullSystemClass;
    

    // State
    USAction_PullInGridContent Action;
    float TimePassed = 0;
    TMap<ACGridContent, ACGridTile> ContentToStartTileMap;
    TArray<ACGridContent> ContentInRange;
    ACGridTile TargetTile;
    ANiagaraActor VortexPullSystem;
    FVector Offset;
    float Size;

    UFUNCTION(BlueprintOverride)
    bool CanVisualizeAction(UCAction inAction)
    {
        return inAction.IsA(USAction_PullInGridContent::StaticClass());
    }

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
        ContentToStartTileMap = Action.ContentToStartTileMap;

        if(ContentInRange.Num() == 0)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "No content in range for VortexPullVisualization");
            return;
        }

        if(ContentInRange.Num() != ContentToStartTileMap.Num())
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "ContentInRange and ContentToStartTileMap have different sizes for VortexPullVisualization");
            return;
        }

        Offset = FVector::ZeroVector;
        Offset.Z = ContentInRange[0].GetActorLocation().Z - ContentInRange[0].GetTile().GetActorLocation().Z;

        const float TileSize = 0.17f; // Size of one tile
        Size = Action.Range * 2 * TileSize + TileSize;

        // Spawn niagara system actor
        VortexPullSystem = Cast<ANiagaraActor>(SpawnActor(VortexPullSystemClass, TargetTile.GetActorLocation() + Offset, FRotator::ZeroRotator));

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

        for (ACGridContent Content : ContentInRange)
        {
            if(IsValid(Content))
            {
                ACGridTile StartTile = ContentToStartTileMap[Content];

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
            for ( auto& pair : ContentToStartTileMap )
            { 
                pair.Value = pair.Key.GetTile();
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