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
    default ArmComp.bDoCollisionTest = false;

    UPROPERTY(DefaultComponent, Attach = ArmComp)
    UCameraComponent Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", Meta = (ClampMin = 1.0f, ClampMax = 100.0f))
    float CameraMoveSpeed = 10.0f;

    //If we haven't manually moved the camera, our TargetLocation will always equal this targets location
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    AActor FollowTarget;

    //The target location we're currently lerping towards.
    FVector TargetLocation;

    //Setting this to false by default to avoid in-editor issues. Enable for builds.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    bool bEdgeOfScreenMouseEnabled = false;

    //How many pixels are considered the edge of the screen for movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    int EdgeOfScreenRadius = 50;

    //Stored reference to avoid casting in Tick 
    APlayerController LocalPC; 


    // -- Mouse scroll wheel dragging movement --

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", meta = (ClampMin = 0.1f, ClampMax = 2.0f))
    float DragSpeed = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", meta = (ClampMin = 0.1f, ClampMax = 5.0f))
    float MaxDragSpeed = 2.0f;

    bool bIsDragging = false;
    FVector2D CurrentMouseDrag = FVector2D::ZeroVector;

    // ------------------------------------------

    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        ACGameState State = Cast<ACGameState>(World.GameState);
        if (State == nullptr) return;

        State.OnTurnOrderUpdate.AddUFunction(this, n"OnTurnOrderUpdate");

        TargetLocation = ActorLocation;
        ActorRotation = FRotator::ZeroRotator;
        FollowActiveUnit();
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
        if (bIsDragging)
            MoveDragMouse();
        else
            MoveMouseEdgeOfScreen();

        if (FollowTarget != nullptr)
        {
            TargetLocation = FollowTarget.ActorLocation;
        }
        ActorLocation = Math::Lerp(ActorLocation, TargetLocation, 0.3f);
    }
    
    void MoveDragMouse()
    {
        Move(FVector2D(CurrentMouseDrag.Y, CurrentMouseDrag.X));
    }

    void MoveMouseEdgeOfScreen()
    {
        if (!bEdgeOfScreenMouseEnabled) return;
        if (Controller == nullptr ||!Controller.IsLocalController()) return;
        if (LocalPC != Controller)
            LocalPC = Cast<APlayerController>(Controller);
        if (LocalPC == nullptr) return;
        
        //Screen coordinates of mouse position in pixels
        float32 MouseX = 0; 
        float32 MouseY = 0;
        if (!LocalPC.GetMousePosition(MouseX, MouseY)) return;

        //Pixel count of viewport
        int ViewX = 0; 
        int ViewY = 0;
        LocalPC.GetViewportSize(ViewX, ViewY);

        FVector2D MouseInput;
        if (MouseX < EdgeOfScreenRadius)
            MouseInput.Y = -1;
        if (MouseX > ViewX - EdgeOfScreenRadius)
            MouseInput.Y = 1;
        if (MouseY < EdgeOfScreenRadius)
            MouseInput.X = 1;
        if (MouseY > ViewY - EdgeOfScreenRadius)
            MouseInput.X = -1;
        
        Move(MouseInput);
    }

    UFUNCTION(BlueprintCallable, Category = "Input|Camera")
    void Move(FVector2D inMovement)
    {
        if (inMovement.IsNearlyZero()) return;

        FVector MoveVector = FVector(inMovement.X, inMovement.Y, 0);
        TargetLocation += MoveVector * CameraMoveSpeed;
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
        if (FollowTarget != nullptr)
            TargetLocation = FollowTarget.ActorLocation;
    }

    UFUNCTION(BlueprintCallable)
    void SetCameraDragging(bool inbIsDragging)
    {
        if (Controller == nullptr ||!Controller.IsLocalController()) return;
        if (LocalPC != Controller)
            LocalPC = Cast<APlayerController>(Controller);
        if (LocalPC == nullptr) return;

        bIsDragging = inbIsDragging;
        CurrentMouseDrag = FVector2D::ZeroVector;
    }

    UFUNCTION(BlueprintCallable)
    void MouseMovement(FVector2D inMovement)
    {
        CurrentMouseDrag += inMovement * DragSpeed;
        if (CurrentMouseDrag.SizeSquared() >= MaxDragSpeed * MaxDragSpeed)
            CurrentMouseDrag = CurrentMouseDrag.GetSafeNormal() * MaxDragSpeed;
    }
}