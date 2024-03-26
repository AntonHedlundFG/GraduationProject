
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CAttributeComponent)

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("game.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	BaseHealth = 100;
	CurrentHealth = BaseHealth;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
}

bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = CurrentHealth;
	float NewHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, BaseHealth);

	float ActualDelta = NewHealth - OldHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, CurrentHealth, ActualDelta);
		}

		//Died.
		if (ActualDelta < 0.0f && CurrentHealth == 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Died!"));
		}
	}

	return ActualDelta != 0;
}

void UCAttributeComponent::ApplyTagWithDuration(FGameplayTag InTag, int Duration)
{
	
}

UCAttributeComponent* UCAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UCAttributeComponent>();
	}

	return nullptr;
}


bool UCAttributeComponent::IsActorAlive(AActor* Actor)
{
	UCAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

//Create deathconsequence.
bool UCAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetBaseHealth());
}


bool UCAttributeComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}


bool UCAttributeComponent::IsFullHealth() const
{
	return CurrentHealth == BaseHealth;
}


float UCAttributeComponent::GetHealth() const
{
	return CurrentHealth;
}


float UCAttributeComponent::GetBaseHealth() const
{
	return BaseHealth;
}


void UCAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void UCAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UCAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAttributeComponent, CurrentHealth);
	DOREPLIFETIME(UCAttributeComponent, BaseHealth);

	DOREPLIFETIME(UCAttributeComponent, Rage);
	DOREPLIFETIME(UCAttributeComponent, RageMax);
}


