class ASPlayerPawn : APawn
{
    // -- Components -- //
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

    // Stored reference to avoid casting in Tick 
    APlayerController LocalPC; 

    // -- Camera Location -- //
    
    // The current mouse movement input
    FVector2D CurrentMouseMoveInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", Meta = (ClampMin = 1.0f, ClampMax = 100.0f))
    float CameraMoveSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera", Meta = (ClampMin = 0.01f, ClampMax = 1.0f))
    float CameraFollowLerpSpeed = 0.3f;

    //If we haven't manually moved the camera, our TargetLocation will always equal this targets location
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    AActor FollowTarget;

    // If we should be dragging the camera around
    UPROPERTY(BlueprintReadWrite)
    bool bIsDragging = false;

    //The target location we're currently lerping towards.
    FVector TargetCameraLocation;

    // -- Edge Scrolling -- //

    // Setting this to false by default to avoid in-editor issues. Enable for builds.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    bool bEdgeOfScreenMouseEnabled = false;

    //How many pixels are considered the edge of the screen for movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera")
    int EdgeOfScreenRadius = 50;

    // -- Camera Location Limits -- //

    // The limits of the camera targets's location in the XYZ of the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|LocationLimits")
    FVector2D CameraXHorizontalLimits = FVector2D(-1000, 1000);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|LocationLimits")
    FVector2D CameraYHorizontalLimits = FVector2D(-1000, 1000);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|LocationLimits")
    FVector2D CameraVerticalLimits = FVector2D(-1000, 1000);

    // -- Camera Rotation --

    // The target rotation we're currently lerping towards.
    FRotator TargetCameraRotation;

    // The speed at which we lerp towards the target rotation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation")
    float CameraRotationLerpSpeed = 0.1f;

    // The speed at which we rotate the target camera rotation based on input
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation")
    FVector2D CameraRotationSpeed = FVector2D(0.1f, 0.1f);

    // If we should clamp the pitch and yaw of the camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation")
    bool bClampCameraPitch = true;

    // The limits of the camera pitch and yaw
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation", Meta = (EditCondition = "bClampCameraPitch"))
    FVector2D CameraVerticalRotationLimits = FVector2D(30.0f, 30.0f);

    // If we should clamp the yaw of the camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation")
    bool bClampCameraYaw = true;

    // The limits of the camera yaw
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Rotation", Meta = (EditCondition = "bClampCameraYaw"))
    FVector2D CameraHorizontalRotationLimits = FVector2D(-30.0f, 30.0f);

    // If we should be rotating the camera
    UPROPERTY(BlueprintReadWrite)
    bool bIsRotating = false;

    // -- Camera Zoom --

    // The speed at which we zoom in and out
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Zoom", Meta = (ClampMin = 100.0f, ClampMax = 2000.0f))
    float CameraZoomSpeed = 100.0f;

    // The limits of the camera zoom (Length of the spring arm)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Camera|Zoom")
    FVector2D CameraZoomLimits = FVector2D(100.0f, 2000.0f);

    // Target zoom length we're currently lerping towards.
    float TargetZoom;

    UPROPERTY(DefaultComponent)
    USmoothDynamicsIntegrator LocationIntegrator;
    UPROPERTY(DefaultComponent)
    USmoothDynamicsIntegrator RotationIntegrator;

    // -- Functions -- //

    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        ACGameState State = Cast<ACGameState>(World.GameState);
        if (State == nullptr) return;

        LocationIntegrator.Initialize(GetActorLocation());
        RotationIntegrator.Initialize(GetActorRotation().Vector());

        State.OnTurnOrderUpdate.AddUFunction(this, n"OnTurnOrderUpdate");

        TargetCameraLocation = ActorLocation;
        TargetZoom = ArmComp.TargetArmLength;
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
        if (bIsRotating) 
        {
            SetTargetCameraRotation();
        } else if(bIsDragging)
        {        
            MoveCameraTarget(FVector2D(CurrentMouseMoveInput.Y, CurrentMouseMoveInput.X));
        } else
        {        
            MoveMouseEdgeOfScreen();
        }

        ClampCameraTargetInBounds();
        ClampTargetCameraRotation();

        FollowCameraTarget(DeltaSeconds);
        FollowCameraRotationTarget(DeltaSeconds);

        FollowZoomTarget();

        // Reset the mouse input
        CurrentMouseMoveInput = FVector2D::ZeroVector; 
    }

    UFUNCTION()
    void SetMouseMovement(FVector2D inMouseMovement)
    {
        CurrentMouseMoveInput = inMouseMovement;
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
        
        MoveCameraTarget(MouseInput);
    }

    UFUNCTION(BlueprintCallable, Category = "Input|Camera")
    void MoveCameraTarget(FVector2D inMovement)
    {
        if (inMovement.IsNearlyZero()) return;

        FVector RotatedVector = FRotator(0, GetActorRotation().Yaw, 0).RotateVector(FVector(inMovement.X, inMovement.Y, 0)); 
        TargetCameraLocation += RotatedVector * CameraMoveSpeed;
        
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
            TargetCameraLocation = FollowTarget.ActorLocation;
    }

    UFUNCTION(BlueprintCallable)
    void SetCameraDragging(bool inbIsDragging)
    {
        if (Controller == nullptr ||!Controller.IsLocalController()) return;
        if (LocalPC != Controller)
            LocalPC = Cast<APlayerController>(Controller);
        if (LocalPC == nullptr) return;

        bIsDragging = inbIsDragging;
    }

    UFUNCTION(BlueprintCallable)
    void Zoom(float inZoomAmount)
    {
       TargetZoom = Math::Clamp(ArmComp.TargetArmLength -inZoomAmount * CameraZoomSpeed, CameraZoomLimits.X, CameraZoomLimits.Y);
    }

    void FollowZoomTarget()
    {
        ArmComp.TargetArmLength = Math::Lerp(ArmComp.TargetArmLength, TargetZoom, CameraFollowLerpSpeed);
    }

    UFUNCTION(BlueprintCallable)
    void SetTargetCameraRotation()
    {
        if (CurrentMouseMoveInput.IsNearlyZero()) return;

        float Yaw = CurrentMouseMoveInput.X;
        float Pitch = CurrentMouseMoveInput.Y;

        Yaw = Yaw * CameraRotationSpeed.X;
        Pitch = Pitch * CameraRotationSpeed.Y;

        // Wrap around when we go over 180 or below -180
        if (TargetCameraRotation.Yaw > 180)
            TargetCameraRotation.Yaw -= 360;
        if (TargetCameraRotation.Yaw < -180)
            TargetCameraRotation.Yaw += 360;

        if (TargetCameraRotation.Pitch > 180)
            TargetCameraRotation.Pitch -= 360; 
        if (TargetCameraRotation.Pitch < -180)
            TargetCameraRotation.Pitch += 360;

        TargetCameraRotation = GetActorRotation() + FRotator(Pitch , Yaw, 0);
    }

    UFUNCTION(BlueprintCallable)
    void ClampTargetCameraRotation()
    {
        if(bClampCameraPitch)
        {        
            TargetCameraRotation.Pitch = Math::ClampAngle(TargetCameraRotation.Pitch, -CameraVerticalRotationLimits.Y + 40, CameraVerticalRotationLimits.X + 30); // Magic numbers to to align with the camera
        }
        if(bClampCameraYaw)
        {        
            TargetCameraRotation.Yaw = Math::Clamp(TargetCameraRotation.Yaw, CameraHorizontalRotationLimits.X, CameraHorizontalRotationLimits.Y);
        }

        TargetCameraRotation.Roll = 0;
    }

    void FollowCameraRotationTarget(float DeltaSeconds)
    {
        FRotator CurrentRotation = GetActorRotation();
        float Yaw = CurrentRotation.Yaw;
        float Pitch = CurrentRotation.Pitch;

        float LerpedYaw = Math::Lerp(Yaw, TargetCameraRotation.Yaw, CameraRotationLerpSpeed);
        float LerpedPitch = Math::Lerp(Pitch, TargetCameraRotation.Pitch, CameraFollowLerpSpeed);

        FVector NewRotationVector = RotationIntegrator.Update(DeltaSeconds, TargetCameraRotation.Vector(), FVector(0));
        FRotator NewRotation = NewRotationVector.Rotation();

        SetActorRotation(NewRotation);
    }

    UFUNCTION(BlueprintCallable)
    void FollowCameraTarget(float DeltaSeconds)
    {      
        if (FollowTarget != nullptr)
        {
            TargetCameraLocation = FollowTarget.ActorLocation;
        }
        FVector NewLocation = LocationIntegrator.Update(DeltaSeconds, TargetCameraLocation, FVector(0));
        SetActorLocation(NewLocation);
    }

    UFUNCTION(BlueprintCallable)
    void ClampCameraTargetInBounds()
    {
        TargetCameraLocation.X = Math::Clamp(TargetCameraLocation.X, CameraXHorizontalLimits.X, CameraXHorizontalLimits.Y);
        TargetCameraLocation.Y = Math::Clamp(TargetCameraLocation.Y, CameraYHorizontalLimits.X, CameraYHorizontalLimits.Y);
        TargetCameraLocation.Z = Math::Clamp(TargetCameraLocation.Z, CameraVerticalLimits.X, CameraVerticalLimits.Y);
    }
}