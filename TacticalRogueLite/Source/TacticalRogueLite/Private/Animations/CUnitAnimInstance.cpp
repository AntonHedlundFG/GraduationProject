// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/CUnitAnimInstance.h"
#include "GameplayTagContainer.h"
#include "Actions/CActionComponent.h"
#include "GameplayTags/SharedGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CUnitAnimInstance)


void UCUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	check(OwningActor);

	ActionComp = OwningActor->FindComponentByClass<UCActionComponent>();
}


void UCUnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ActionComp)
	{
		bIsMoving = ActionComp->ActiveGameplayTags.HasTag(Tag_Action_Moving); //Should maybe be eventdriven instead. 
	}
}
