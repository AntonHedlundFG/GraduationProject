// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CUnitAnimInstance.generated.h"

class UCActionComponent;

UCLASS()
class TACTICALROGUELITE_API UCUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	//Is Unit walking based on GameplayTag data.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UCActionComponent> ActionComp;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
