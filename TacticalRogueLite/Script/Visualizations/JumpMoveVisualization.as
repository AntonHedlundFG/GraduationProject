class USJumpMoveVisualization : UCActionVisualization
{

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UndoSpeedMultiplier = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PeakHeight = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ActorOffset;
    // State
    USMoveAction_Jumping JumpAction;
    float TimePassed = 0;
    FVector StartLocation;
    FVector EndLocation;
    AActor Actor;

    UFUNCTION(BlueprintOverride)
    bool CanVisualizeAction(UCAction Action)
    {
        return Action.IsA(USMoveAction_Jumping::StaticClass());
    }

    UFUNCTION(BlueprintOverride)
    void Enter()
    {
        TimePassed = 0;
        JumpAction = Cast<USMoveAction_Jumping>(VisualizedAction);

        if(!IsValid(JumpAction))
        {
            return;
        }

        Actor = JumpAction.MovedUnit;

        if(!IsValid(Actor))
        {
            return;
        }
        StartLocation = Actor.GetActorLocation();
        ActorOffset =  StartLocation - JumpAction.FromTile.GetActorLocation();
        EndLocation = JumpAction.TargetTile.GetActorLocation() + ActorOffset;
    }

    UFUNCTION(BlueprintOverride)
    bool Tick(float DeltaTime)
    {
        TimePassed = Math::Clamp(TimePassed + DeltaTime, 0, Duration);

        if(!IsValid(JumpAction) || !IsValid(Actor))
        {
            return false;
        }

        FVector NewLocation = Math::Lerp(StartLocation, EndLocation, TimePassed / Duration);
        NewLocation.Z = Math::Sin(TimePassed / Duration * PI) * PeakHeight + ActorOffset.Z;

        Actor.SetActorLocation(NewLocation);

        return DeltaTime >= 0 ? TimePassed >= Duration : TimePassed <= 0;
    }

    UFUNCTION(BlueprintOverride)
    bool RevertTick(float DeltaTime)
    {
        return Tick(-DeltaTime * UndoSpeedMultiplier);
    }

}