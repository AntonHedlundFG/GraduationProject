#include "Actions/CActionComponent.h"
#include "Logging/StructuredLog.h"
#include "Actions/CAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "GridContent/CUnit.h"
#include "TacticalRogueLite/TacticalRogueLite.h"


UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UCActionComponent::AddAbility(FAbility Ability)
{
	RemoveAbility(Ability.InventorySlotTag);
	for (TSubclassOf<UCAction> ActionClass : Ability.Actions)
	{
		UCAction* NewInstancedAction = NewObject<UCAction>(GetOwner(), ActionClass);
		NewInstancedAction->Initialize(this);
		
		Ability.AbilityTags.AppendTags(NewInstancedAction->GetActionTags());
		
		Ability.InstantiatedActions.Add(NewInstancedAction);
	}
	Abilities.Add(Ability);
}

void UCActionComponent::RemoveAbility(FGameplayTag ItemSlot)
{
	for (FAbility& Ability : Abilities)
	{
		if (Ability.InventorySlotTag == ItemSlot)
		{
			Abilities.Remove(Ability); // Remove the ability from the array leaving it out of scope
			return;
		}
	}
}

bool UCActionComponent::TryGetAbility(FGameplayTag ItemSlot, FAbility& outAbility)
{
	for (FAbility& Ability : Abilities)
	{
		if (Ability.InventorySlotTag == ItemSlot)
		{
			outAbility = Ability;
			return true;
		}
	}
	return false;
}

TArray<ACGridTile*> UCActionComponent::GetValidTargetTiles(FGameplayTag itemSlot)
{
	ACUnit* Owner = Cast<ACUnit>(GetOwner());
	FAbility OutAbility;
	if (!TryGetAbility(itemSlot, OutAbility) || !Owner)
		return TArray<ACGridTile*>();

	return OutAbility.GetValidTargetTiles(Owner->GetTile());
}

bool UCActionComponent::IsValidTargetTile(FGameplayTag ItemSlot, ACGridTile* TargetTile)
{
	return GetValidTargetTiles(ItemSlot).Contains(TargetTile);
}

bool UCActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FAbility& Ability : Abilities)
	{
		for (UCAction* Action : Ability.InstantiatedActions)
		{
			if (Action)
				WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


void UCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCActionComponent, Abilities);
}
