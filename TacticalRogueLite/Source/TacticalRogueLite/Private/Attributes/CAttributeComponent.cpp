
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
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	
	int OldHealth = CurrentHealth;
	int NewHealth = (CurrentHealth + Delta > BaseHealth)? BaseHealth:(CurrentHealth + Delta < 0)? 0: CurrentHealth + Delta;
	
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
				bIsPendingKill = true;
			}
		}
	}

	return Delta != 0;
}

void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	UsedItemCharges.OnStackChanged.AddDynamic(this, &UCAttributeComponent::BroadcastOnItemChargesChanged);
	MaxItemCharges.OnStackChanged.AddDynamic(this, &UCAttributeComponent::BroadcastOnItemChargesChanged);
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
	return CurrentHealth > 0.0f && !bIsPendingKill;
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

float UCAttributeComponent::GetHealthPercent() const
{
	return GetHealth() / static_cast<float>(GetBaseHealth());
}

int UCAttributeComponent::GetMovement() const
{
	return CurrentMovement;
}

void UCAttributeComponent::SetMovement(int NewMovement)
{
	CurrentMovement = NewMovement;
}

void UCAttributeComponent::SetBaseMovement(int NewBaseMovement)
{
	//BaseMovement == NewBaseMovement;
}

bool UCAttributeComponent::IsBaseMovement() const
{
	return CurrentMovement == BaseMovement;
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
	DOREPLIFETIME(UCAttributeComponent, GameplayStats);
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
	UsedItemCharges.ClearAllStacks();
}

void UCAttributeComponent::BroadcastOnItemChargesChanged()
{
	OnItemChargesChanged.Broadcast();
}

#pragma endregion

