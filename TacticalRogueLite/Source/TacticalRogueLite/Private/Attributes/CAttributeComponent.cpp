
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
	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
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

void UCAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAttributeComponent, ActiveGameplayTags);
	DOREPLIFETIME(UCAttributeComponent, MaxItemCharges);
	DOREPLIFETIME(UCAttributeComponent, UsedItemCharges);
	DOREPLIFETIME(UCAttributeComponent, GameplayStats);
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

