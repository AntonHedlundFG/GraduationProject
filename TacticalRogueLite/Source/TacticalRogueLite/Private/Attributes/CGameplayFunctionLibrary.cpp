// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/CGameplayFunctionLibrary.h"
#include "Attributes/CAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CGameplayFunctionLibrary)


bool UCGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, int DamageAmount)
{
	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UCGameplayFunctionLibrary::ApplyHealing(AActor* Insitgator, AActor* Target, int Amount)
{
	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(Target);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(Insitgator, Amount);
	}
	return false;
}

// int32 USGameplayFunctionLibrary::GetRemainingBundledPSOs()
// {
// 	// Counts Bundled PSOs remaining, exposed for UI access
// 	return FShaderPipelineCache::NumPrecompilesRemaining();
// }