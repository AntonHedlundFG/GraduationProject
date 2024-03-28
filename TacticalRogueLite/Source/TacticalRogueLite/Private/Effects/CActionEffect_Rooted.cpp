// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CActionEffect_Rooted.h"
#include "Attributes/CAttributeComponent.h"
#include "Actions/CActionComponent.h"
//#include "CGameplayFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CActionEffect_Rooted)


UCActionEffect_Rooted::UCActionEffect_Rooted()
{
	ReflectFraction = 0.2f;
}


void UCActionEffect_Rooted::StartAction_Implementation(AActor* Instigator)
{//Grant tag = status.rooted
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UCActionEffect_Rooted::OnHealthChanged);
		
	}
}


void UCActionEffect_Rooted::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UCActionEffect_Rooted::OnHealthChanged);
	}
}


void UCActionEffect_Rooted::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		//USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}