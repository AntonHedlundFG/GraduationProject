
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

void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_ListenServer)
	{
		FGameplayTagContainer AllItemTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(SharedGameplayTags::ItemSlot);
		for (FGameplayTag ItemTag : AllItemTags)
		{
			AddMaxCharges(ItemTag, 2);
		}
	}
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
	DOREPLIFETIME(UCAttributeComponent, MaxItemCharges);
	DOREPLIFETIME(UCAttributeComponent, UsedItemCharges);

	DOREPLIFETIME(UCAttributeComponent, Rage);
	DOREPLIFETIME(UCAttributeComponent, RageMax);
}



#pragma region Item Charges

int32 UCAttributeComponent::GetRemainingCharges(FGameplayTag ItemSlot)
{
	return MaxItemCharges.GetStackCount(ItemSlot) - UsedItemCharges.GetStackCount(ItemSlot);
}

bool UCAttributeComponent::TrySpendCharge(FGameplayTag ItemSlot, int32 Amount /* = 1 */)
{
	if (GetRemainingCharges(ItemSlot) < Amount)
		return false;
	
	UsedItemCharges.AddStackCount(ItemSlot, Amount);
	return true;
}

bool UCAttributeComponent::TryUndoSpendCharge(FGameplayTag ItemSlot, int32 Amount /* = 1 */)
{
	if (UsedItemCharges.GetStackCount(ItemSlot) < Amount)
		return false;

	UsedItemCharges.RemoveStackCount(ItemSlot, Amount);
	return true;
}

void UCAttributeComponent::AddMaxCharges(FGameplayTag ItemSlot, int32 Amount)
{
	MaxItemCharges.AddStackCount(ItemSlot, Amount);
}

void UCAttributeComponent::RemoveMaxCharges(FGameplayTag ItemSlot, int32 Amount)
{
	MaxItemCharges.RemoveStackCount(ItemSlot, Amount);
}

void UCAttributeComponent::ResetSpentCharges()
{
	//TODO: Make a clear all in attributeutility.
}

#pragma endregion

