
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CAttributeComponent)

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("game.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
<<<<<<< HEAD
	HealthMax = 100;
	Health = HealthMax;
=======
	BaseHealth = 100;
	CurrentHealth = BaseHealth;
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
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

<<<<<<< HEAD
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
=======
	float OldHealth = CurrentHealth;
	float NewHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, BaseHealth);
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d

	float ActualDelta = NewHealth - OldHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
<<<<<<< HEAD
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		//Died.
		if (ActualDelta < 0.0f && Health == 0.0f)
=======
		CurrentHealth = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, CurrentHealth, ActualDelta);
		}

		//Died.
		if (ActualDelta < 0.0f && CurrentHealth == 0.0f)
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
		{
			UE_LOG(LogTemp, Warning, TEXT("Died!"));
		}
	}

	return ActualDelta != 0;
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

<<<<<<< HEAD

=======
//Create deathconsequence
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
bool UCAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool UCAttributeComponent::IsAlive() const
{
<<<<<<< HEAD
	return Health > 0.0f;
=======
	return CurrentHealth > 0.0f;
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
}


bool UCAttributeComponent::IsFullHealth() const
{
<<<<<<< HEAD
	return Health == HealthMax;
=======
	return CurrentHealth == BaseHealth;
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
}


float UCAttributeComponent::GetHealth() const
{
<<<<<<< HEAD
	return Health;
=======
	return CurrentHealth;
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
}


float UCAttributeComponent::GetHealthMax() const
{
<<<<<<< HEAD
	return HealthMax;
=======
	return BaseHealth;
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
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

<<<<<<< HEAD
	DOREPLIFETIME(UCAttributeComponent, Health);
	DOREPLIFETIME(UCAttributeComponent, HealthMax);
=======
	DOREPLIFETIME(UCAttributeComponent, CurrentHealth);
	DOREPLIFETIME(UCAttributeComponent, BaseHealth);
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d

	DOREPLIFETIME(UCAttributeComponent, Rage);
	DOREPLIFETIME(UCAttributeComponent, RageMax);
}


