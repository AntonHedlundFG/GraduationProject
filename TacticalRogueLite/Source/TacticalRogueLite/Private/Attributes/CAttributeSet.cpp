// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/CAttributeSet.h"
#include "Actions/CActionComponent.h"
#include "Attributes/Utilities/CAttribute.h"

void UCAttributeSet::Initialize(UCActionComponent* NewOwner)
 {
 	OwningComp = NewOwner;

 	K2_Initialize();
 }

UCActionComponent* UCAttributeSet::GetOwningComponent() const
{
	return OwningComp;
}


bool UCAttributeSet::GetAttribute(FGameplayTag InTag, FAttribute& OutAttribute)
{

	FName PropertyName;
	if(GetAttributeName(InTag, PropertyName))
	{
		FStructProperty* Prop = CastField<FStructProperty>(GetClass()->FindPropertyByName(PropertyName));
		if (Prop)
		{
			FAttribute* FoundAttribute = Prop->ContainerPtrToValuePtr<FAttribute>(this);
			OutAttribute = *FoundAttribute;
			return true;
		}
	}

	//Failed to find attribute based on gameplaytag.
	//LOG_WARNING("Attributeset failed to find attribute", PropertyName, "in", OwningComp->GetOwner());
	return false;
}


//Property system- Find stuff in runtime by specifying name. In this case attributetag. (Attribute_Name).
int UCAttributeSet::ApplyAttributeChange(const FAttributeModification& ModToApply, int32 Level)
{
	FName PropertyName;
	if (GetAttributeName(ModToApply.AttributeTag, PropertyName))
	{
		FStructProperty* Prop = CastField<FStructProperty>(GetClass()->FindPropertyByName(PropertyName));
		if(ensure(Prop)) //On failure: We dont have the attribute in this set.
		{
			FAttribute* FoundAttribute = Prop->ContainerPtrToValuePtr<FAttribute>(this);
			const int OldValue = FoundAttribute->GetValue();
			const int Delta = ModToApply.GetMagnitude(Level);

			//Apply the basic modification of attribute.
			switch (ModToApply.ModifierOperation)
			{
			case EAttributeModifierOperation::AddDelta:
				FoundAttribute->Delta += Delta;
				if (FoundAttribute->bClampToZero)
				{
					FoundAttribute->Delta = FMath::Max(FoundAttribute->Delta, 0);
				}
				break;
			case EAttributeModifierOperation::AddBase:
				FoundAttribute->BaseValue += Delta;
				if (FoundAttribute->bClampToZero)
				{
					FoundAttribute->BaseValue = FMath::Max(FoundAttribute->BaseValue, 0);
				}
				break;
			case EAttributeModifierOperation::AddMultiplier:
				FoundAttribute->Multiplier += Delta;
				if (FoundAttribute->bClampToZero)
				{
					FoundAttribute->Multiplier = FMath::Max(FoundAttribute->Multiplier, 0);
				}
				break;
			case EAttributeModifierOperation::OverrideBase:
				FoundAttribute->BaseValue = Delta;
				if (FoundAttribute->bClampToZero)
				{
					FoundAttribute->Multiplier = FMath::Max(FoundAttribute->BaseValue, 0);
				}
				break;
			}

			//Allow game-wide rules to react to attribute changes such as clamping Health to HealthMax or +3 to Strength givinh you more Health as result.
			PostAttributeChanged(ModToApply, Level);

			//Notify any listeners an attribute changed.
			//GetOwningComponent()->BroadCastAttributeChanged(ModToApply.AttributeTag, ModToApply.InstigatorComp.Get(), FoundAttribute->GetValue(), Delta, ModToApply.AddedTags, ModToApply.ModifierOperation);

			return Delta;
 		}
 	}

 	//Failed to find attribute.
 	return 0;
 }


//"Post process". After eg. health changed, we check if the Basevalue is below zero and adds a "Death tag". If healthmax gets overriden(init), it automatically sets the new value.
void UCAttributeSet::PostAttributeChanged_Implementation(const FAttributeModification& AppliedMod, int32 Level)
{
	if (AppliedMod.AttributeTag == FGameplayTag::RequestGameplayTag("Attribute.Health"))
	{

		//Sanity check that we change the BASE and clamp to that... In theory its a little more complex to clamp hp to both base and delta against maxHP.(float?).
		ensure(AppliedMod.ModifierOperation != EAttributeModifierOperation::AddDelta);

		//Clamp the change to health.
		Health.BaseValue = FMath::Clamp(Health.BaseValue, 0, HealthMax.GetValue() * HealthMaxModifier.GetValue());

		if (Health.BaseValue == 0)
		{
			FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("Unit.Killed");
			//FGameplayTagContainer OwnedTags = OwningComp->GetActiveTags();

			//*Revival isnt implemented here. Anyone reviving by adding health should consider this and handle the death tag itself.
			// if (!OwnedTags.HasTag(DeathTag))
			// {
			// 	OwningComp->AddGameplayTag(DeathTag);
			// }
		}
	}
	if (AppliedMod.AttributeTag == FGameplayTag::RequestGameplayTag("Attribute.HealthMax"))
	{
		if (AppliedMod.ModifierOperation == EAttributeModifierOperation::OverrideBase)
		{
			//Set Health to HealthMax (Expected only at initialization, gameplay changes need to use AddBase/AddDelta instead.
			FAttributeModification HealthMod;
			HealthMod.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
			HealthMod.Magnitude = AppliedMod.GetMagnitude(Level);
			HealthMod.ModifierOperation = EAttributeModifierOperation::OverrideBase;
			HealthMod.AddedTags = AppliedMod.AddedTags;
			ApplyAttributeChange(HealthMod);
		}
		else
		{
			//On positive change, add same amount to HEALTH.
			if (AppliedMod.Magnitude > 0)
			{
				FAttributeModification HealthMod;
				HealthMod.AttributeTag = FGameplayTag::RequestGameplayTag("Attribute.Health");
				HealthMod.Magnitude = AppliedMod.GetMagnitude(Level);
				HealthMod.ModifierOperation = EAttributeModifierOperation::AddBase;
				HealthMod.AddedTags = AppliedMod.AddedTags;
				ApplyAttributeChange(HealthMod);
			}
			else
			{
				//Re-clamp Health now that MaxHealth has reduced.
				Health.BaseValue = FMath::Clamp(Health.BaseValue, 0, HealthMax.GetValue() * HealthMaxModifier.GetValue());
			}
		}
	}

	//...Add the other reactions here...
}

int UCAttributeSet::GetAttributeValue(FGameplayTag InTag)
{
	FAttribute Attribute;
	GetAttribute(InTag, Attribute);
	return Attribute.GetValue();
}


bool UCAttributeSet::GetAttributeName(FGameplayTag InTag, FName& OutAttributeName)
{
	if (!InTag.IsValid())
	{
		return false;
	}

	//Attribute names should reflect the Tag name in project. eg. Grab "Health" (property name) from Attribute.Health in GameplayTag.
	FName TagName = InTag.GetTagName();
	FString LeftStr;
	FString RightStr;
	TagName.ToString().Split(".", &LeftStr, &RightStr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (RightStr.IsEmpty())
	{
		LOG_WARNING("Failed to split GameplayTag");
		return false;
	}

	OutAttributeName = FName(*RightStr);
	return true;
}

void UCAttributeSet::OnRep_OwningComp()
{
	//...TODO: Fix
}

void UCAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	//TODO: Fix
}

void UCAttributeSet::OnRep_Health(FAttribute OldAttribute)
 {
 	AttributeChangedFromReplication("Attribute.Health", Health, OldAttribute);
 }

void UCAttributeSet::OnRep_HealthMax(FAttribute OldAttribute)
 {
 	AttributeChangedFromReplication("Attribute.HealthMax", HealthMax, OldAttribute);
 }

void UCAttributeSet::OnRep_HealthModifier(FAttribute OldAttribute)
{
	AttributeChangedFromReplication("Attribute.HealthModifier", HealthMaxModifier, OldAttribute);
}

void UCAttributeSet::AttributeChangedFromReplication(FName AttributeName, FAttribute NewAttribute,
	FAttribute OldAttribute)
{
	//TODO: Fix.
}







