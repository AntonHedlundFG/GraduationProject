
#include "Attributes/CAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actions/CDeathAction.h"
#include "CGameMode.h"
#include "GamePlayTags/SharedGamePlayTags.h"
//variablar onrep istället
//viktigt att replikera till clienterna- tags osv

#include UE_INLINE_GENERATED_CPP_BY_NAME(CAttributeComponent)


UCAttributeComponent::UCAttributeComponent()
{
	BaseHealth = 100;
	CurrentHealth = BaseHealth;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
}

bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, int Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0)
	{
		return false;
	}

	
	int OldHealth = CurrentHealth;
	int NewHealth = CurrentHealth + Delta;
	
	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = NewHealth;

		if (Delta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, CurrentHealth, Delta);
		}

		//Died.
		if (OldHealth > 0 && NewHealth <= 0)
		{
			UCDeathAction* DeathAction = NewObject<UCDeathAction>(GetOuter(), UCDeathAction::StaticClass());
			DeathAction->AffectedUnit = Cast<ACUnit>(GetOwner());
			ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
			if (GameMode)
			{
				GameMode->RegisterAction(DeathAction);
			}
		}
	}

	return Delta != 0;
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


int UCAttributeComponent::GetHealth() const
{
	return CurrentHealth;
}

void UCAttributeComponent::SetHealth(int NewHealth)
{
	CurrentHealth = NewHealth;
}


int UCAttributeComponent::GetBaseHealth() const
{
	return BaseHealth;
}


void UCAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, int NewHealth, int Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void UCAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAttributeComponent, CurrentHealth);
	DOREPLIFETIME(UCAttributeComponent, BaseHealth);
	DOREPLIFETIME(UCAttributeComponent, ActiveGameplayTags);

	DOREPLIFETIME(UCAttributeComponent, Rage);
	DOREPLIFETIME(UCAttributeComponent, RageMax);
}



#pragma region Item Charges

uint8 UCAttributeComponent::RemainingCharges(FGameplayTag ItemSlot)
{
	uint8 MaxCharges = GetMaxChargesRef(ItemSlot);
	uint8 CurrentUses = GetUsedChargesRef(ItemSlot);
	return CurrentUses > MaxCharges ? 0 : MaxCharges - CurrentUses;
}

bool UCAttributeComponent::TrySpendCharge(FGameplayTag ItemSlot, uint8 Amount /* = 1 */)
{
	if (RemainingCharges(ItemSlot) < Amount)
		return false;

	GetUsedChargesRef(ItemSlot) += Amount;
	return true;
}

bool UCAttributeComponent::TryUndoSpendCharge(FGameplayTag ItemSlot, uint8 Amount /* = 1 */)
{
	if (GetUsedChargesRef(ItemSlot) < Amount)
		return false;
	GetUsedChargesRef(ItemSlot) -= Amount;
	return true;
}

void UCAttributeComponent::AddMaxCharges(FGameplayTag ItemSlot, uint8 Amount)
{
	GetMaxChargesRef(ItemSlot) += Amount;
}

void UCAttributeComponent::RemoveMaxCharges(FGameplayTag ItemSlot, uint8 Amount)
{
	uint8& MaxChargesRef = GetMaxChargesRef(ItemSlot);
	MaxChargesRef = Amount >= MaxChargesRef ? 0 : MaxChargesRef - Amount;
}

uint8& UCAttributeComponent::GetMaxChargesRef(FGameplayTag ItemSlot)
{
	if (ItemSlot == SharedGameplayTags::ItemSlot_Boots)
	{
		return BootsCharges;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Armor)
	{
		return ArmorCharges;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Weapon)
	{
		return WeaponCharges;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Trinket)
	{
		return TrinketCharges;
	}

	LOG_ERROR("Accessing item charge using wrong tag. Defaulting to Boots reference. %s", *ItemSlot.ToString());
	return BootsCharges;
}

uint8& UCAttributeComponent::GetUsedChargesRef(FGameplayTag ItemSlot)
{
	if (ItemSlot == SharedGameplayTags::ItemSlot_Boots)
	{
		return BootsUsed;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Armor)
	{
		return ArmorUsed;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Weapon)
	{
		return WeaponUsed;
	}
	if (ItemSlot == SharedGameplayTags::ItemSlot_Trinket)
	{
		return TrinketUsed;
	}

	LOG_ERROR("Accessing item usage using wrong tag. Defaulting to Boots reference. %s", *ItemSlot.ToString());
	return BootsUsed;
}

#pragma endregion

