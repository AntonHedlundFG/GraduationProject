// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/CGameplayFunctionLibrary.h"
#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CGameplayFunctionLibrary)


FAttributeModifications UCGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, int DamageAmount, FGameplayTagContainer ContextTags)
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
		FAttribute ArmorAttribute;
		ActionComp->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.Armor"),ArmorAttribute);

		int DamageToArmor = FMath::Min(ArmorAttribute.BaseValue, DamageAmount);
		int DamageToHealth = DamageAmount - DamageToArmor;
	
		FAttributeModification HealthModification;
		FAttributeModification ArmorModification;

		HealthModification.InstigatorComp = DamageCauser->GetComponentByClass<UCActionComponent>();
		HealthModification.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
		HealthModification.ModifierOperation = EAttributeModifierOperation::AddBase;
		HealthModification.Magnitude = -DamageToHealth;

		ArmorModification.InstigatorComp = DamageCauser->GetComponentByClass<UCActionComponent>();
		ArmorModification.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Armor");
		ArmorModification.ModifierOperation = EAttributeModifierOperation::AddBase;
		ArmorModification.Magnitude = -DamageToArmor;

		FAttributeModifications ReturnUndoModifications;
		
		if (DamageToArmor > 0)
		{
			ActionComp->ApplyAttributeChange(ArmorModification, 0);
			FAttributeModification ReturnUndoArmorMod = ArmorModification;
			ReturnUndoArmorMod.Magnitude = -ReturnUndoArmorMod.Magnitude;
			ReturnUndoArmorMod.bIsUndo = true;
			ReturnUndoModifications.Modifications.Add(ReturnUndoArmorMod);
		}
		if (DamageToHealth > 0)
		{
			ActionComp->ApplyAttributeChange(HealthModification, 0);
			FAttributeModification ReturnUndoHealthMod = HealthModification;
			ReturnUndoHealthMod.Magnitude = -ReturnUndoHealthMod.Magnitude;
			ReturnUndoHealthMod.bIsUndo = true;
			ReturnUndoModifications.Modifications.Add(ReturnUndoHealthMod);
		}
		
		//Modification.AddedTags = ContextTags;
		//FMath::Abs(ActualDamage); //?
		//ActionComp->BroadcastAttributeChanged(Modification.AttributeTag, UCActionComponent::GetActionComp(DamageCauser), )
		return ReturnUndoModifications;
	}

	return FAttributeModifications();
}

FGameplayTagContainer UCGameplayFunctionLibrary::ToGameplayTagContainer( const FGameplayTagStackContainer& StackedContainer )
{
	FGameplayTagContainer NewContainer;
	for ( const auto& TagPair : StackedContainer.GetTagCountMap() )
	{
		NewContainer.AddTag(TagPair.Key);
	}
	return NewContainer;
}

/* Commenting this out since we want to use the returned array of FAttributeModifications for undoing damage...
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
		//Modification.AddedTags = ContextTags;
		ActionComp->ApplyAttributeChange(Modification, 0);
		return true;
	}
	
	return false;
}
*/
// int32 USGameplayFunctionLibrary::GetRemainingBundledPSOs()
// {
// 	// Counts Bundled PSOs remaining, exposed for UI access
// 	return FShaderPipelineCache::NumPrecompilesRemaining();
// }