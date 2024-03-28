
#include "AliceTest/CAliceTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actions/CActionComponent.h"
#include "GamePlayTags/SharedGamePlayTags.h"

// Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AliceTest/CAliceTestPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CAliceTestCharacter)

// Sets default values
ACAliceTestCharacter::ACAliceTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	ActionComp = CreateDefaultSubobject<UCActionComponent>("ActionComp");
	
	
}


void ACAliceTestCharacter::PrimaryAttack()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("ATTACK")));
	if(!ActionComp) return;
	
	//ActionComp->StartActionByName(this, SharedGameplayTags::Action_DefaultAttack);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("Action Comp")));
}

// Called when the game starts or when spawned
void ACAliceTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACAliceTestCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ACAliceTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACAliceTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

	//PlayerInputComponent->BindAction(

	APlayerController* PC = GetController<APlayerController>();

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(AliceMapping, 0);
	
	//const ULocalPlayer* LP = PC->GetLocalPlayer();
	//
	//UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	//check(Subsystem);
	//
	//Subsystem->ClearAllMappings();

	// Add mappings, runtime remove/add(?).
	//Subsystem->AddMappingContext(AliceMapping, 0);

	//New Enhanced Input system.
	UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	InputComp->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACAliceTestCharacter::PrimaryAttack);

	
}

