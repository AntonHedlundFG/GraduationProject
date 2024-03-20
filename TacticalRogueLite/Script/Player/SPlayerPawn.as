class ASPlayerPawn : APawn
{
    UPROPERTY(DefaultComponent, RootComponent)
    USceneComponent RootComp;

    UPROPERTY(DefaultComponent)
    UStaticMeshComponent VisualMesh;
    default VisualMesh.bHiddenInGame = true;
    default VisualMesh.CollisionEnabled = ECollisionEnabled::NoCollision;

    UPROPERTY(DefaultComponent)
    USpringArmComponent ArmComp;
    default ArmComp.TargetArmLength = 1000;
    default ArmComp.RelativeRotation = FRotator(-75, 0, 0);

    UPROPERTY(DefaultComponent, Attach = ArmComp)
    UCameraComponent Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", Meta = (ClampMin = 0.1f, ClampMax = 100.0f))
    float CameraMoveSpeed = 10.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    AActor FollowTarget;

    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        ACGameState State = Cast<ACGameState>(World.GameState);
        if (State == nullptr) return;
        State.OnTurnOrderUpdate.AddUFunction(this, n"OnTurnOrderUpdate");
    }
    UFUNCTION(BlueprintOverride)
    void EndPlay(EEndPlayReason EndPlayReason)
    {
        ACGameState State = Cast<ACGameState>(World.GameState);
        if (State == nullptr) return;
        State.OnTurnOrderUpdate.UnbindObject(this);
    }

    UFUNCTION(BlueprintOverride)
    void Tick(float DeltaSeconds)
    {
        if (FollowTarget != nullptr)
        {
            ActorLocation = Math::Lerp(ActorLocation, FollowTarget.ActorLocation, 0.3f);
        }
    }
    
    UFUNCTION(BlueprintCallable, Category = "Input|Camera")
    void Move(FVector2D inMovement)
    {
        if (inMovement.IsNearlyZero()) return;

        FVector MoveVector = FVector(inMovement.X, inMovement.Y, 0);
        ActorLocation += MoveVector * CameraMoveSpeed;
        if (FollowTarget != nullptr)
        {
            FollowTarget = nullptr;
        }
    }

    UFUNCTION()
    private void OnTurnOrderUpdate()
    {
        FollowActiveUnit();
    }

    UFUNCTION(BlueprintCallable)
    void FollowActiveUnit()
    {
        ACGameState State = Cast<ACGameState>(World.GameState);
        if (State == nullptr || State.TurnOrder.Num() == 0) return;

        FollowTarget = State.TurnOrder[0];
    }
}