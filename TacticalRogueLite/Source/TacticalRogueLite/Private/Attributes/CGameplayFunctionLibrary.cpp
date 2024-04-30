// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/CGameplayFunctionLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CGameplayFunctionLibrary)


int UCGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, int DamageAmount, FGameplayTagContainer ContextTags)
{
	// UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(TargetActor);
	// if (AttributeComp)
	// {
	// 	return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	// }

	UCActionComponent* ActionComp = TargetActor->GetComponentByClass<UCActionComponent>();
	//if (UCActionComponent* ActionComp = UCActionComponent::GetActionComp(TargetActor))
	if (ActionComp)
	{
		int Amount = DamageAmount / -1;
	
		FAttributeModification Modification;

		Modification.InstigatorComp = DamageCauser->GetComponentByClass<UCActionComponent>();
		Modification.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
		Modification.ModifierOperation = EAttributeModifierOperation::AddBase;
		Modification.Magnitude = Amount;
		Modification.AddedTags = ContextTags;
		int ActualDamage = ActionComp->ApplyAttributeChange(Modification, 0);
		//FMath::Abs(ActualDamage); //?
		//ActionComp->BroadcastAttributeChanged(Modification.AttributeTag, UCActionComponent::GetActionComp(DamageCauser), )
		return ActualDamage;
	}

	return 0;
}

bool UCGameplayFunctionLibrary::UndoDamage(AActor* InstigatorActor, AActor* TargetActor, int Amount, FGameplayTagContainer ContextTags)
{
	// UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(Target);
	// if (AttributeComp)
	// {
	// 	return AttributeComp->ApplyHealthChange(Insitgator, Amount);
	// }
	// return false;

	UCActionComponent* ActionComp = TargetActor->GetComponentByClass<UCActionComponent>();
	//if (UCActionComponent* ActionComp = UCActionComponent::GetActionComp(TargetActor))
	if (ActionComp)
	{
		FAttributeModification Modification;
		Modification.InstigatorComp = ActionComp;
		Modification.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
		Modification.ModifierOperation = EAttributeModifierOperation::AddBase;
		Modification.Magnitude = -Amount;
		Modification.AddedTags = ContextTags;
		ActionComp->ApplyAttributeChange(Modification, 0);
		return true;
	}
	
	return false;
}

// int32 USGameplayFunctionLibrary::GetRemainingBundledPSOs()
// {
// 	// Counts Bundled PSOs remaining, exposed for UI access
// 	return FShaderPipelineCache::NumPrecompilesRemaining();
// }