
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputAction.h"
#include "CAliceTestCharacter.generated.h"
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UCActionComponent;


UCLASS()
class TACTICALROGUELITE_API ACAliceTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACAliceTestCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> AliceMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCActionComponent> ActionComp;
	
	//Enhanced Input.
	//Three parameter options available (FInputActionInstance, FInputActionValue, or none).

	void PrimaryAttack();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
