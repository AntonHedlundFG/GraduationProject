#include "Actions/CActionComponent.h"
#include "Actions/CAction.h"
#include "Attributes/Utilities/CAttribute.h"
#include "Attributes/CAttributeSet.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "GridContent/CUnit.h"


UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		if (AttributeClass)
		{
			AttributeSet = NewObject<UCAttributeSet>(this, AttributeClass);
			check(AttributeSet);
			AttributeSet->Initialize(this);
			//LOG_WARNING("Setup Attributeset!");
		}
		//LOG_WARNING("No Attributeset!");
	}
}

void UCActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UCActionComponent::GetAttribute(FGameplayTag InTag, FAttribute& InAttribute)
{
	if (!AttributeSet)
	{
		LOG_WARNING("AttributeSet is not valid.");
		return false;
	}
	
	return AttributeSet->GetAttribute(InTag, InAttribute);
}

bool UCActionComponent::K2_GetAttribute(FGameplayTag AttributeTag, int& CurrentValue, int& BaseValue, int& Additive,
                                        int& Multiplier)
{
	// Create a temporary FAttribute variable to hold the retrieved attribute.
	FAttribute Attribute;

	// Call the GetAttribute function of the AttributeSet to retrieve the attribute data
	bool bSuccess = GetAttribute(AttributeTag, Attribute);

	// If attribute retrieval was successful, populate the output parameters with attribute data
	if (bSuccess)
	{
		CurrentValue = Attribute.GetValue();  // Get the current value of the attribute
		BaseValue = Attribute.BaseValue;      // Get the base value of the attribute
		Additive = Attribute.Delta;           // Get the additive value of the attribute
		Multiplier = Attribute.Multiplier;    // Get the multiplier value of the attribute
	}

	return bSuccess;
}

int UCActionComponent::ApplyAttributeChange(const FAttributeModification& InAttributeMod, int32 Level)
{
	//Applies the attribute, triggers events and allows other attributes to react to the change.
	//eg. +3 Strength might change the HealthMax too in RPG style.
	return AttributeSet->ApplyAttributeChange(InAttributeMod, Level);
}


bool UCActionComponent::CanApplyAttributeModifiers(UCActionWithTimer* Effect)
{
	for (const FAttributeModification& Mod : Effect->ModifiersAppliedToOwner)
	{
		FAttribute AttributeToMod;
		GetAttribute(Mod.AttributeTag, AttributeToMod);

		//Only makes sense to check additive modifiers.
		if (ensureAlways(Mod.ModifierOperation == EAttributeModifierOperation::AddDelta))
		{
			if (Mod.GetMagnitude(Effect->GetLevel()) < 0 )
			{
				//This assumes attributes can never go below zero when used in this context. eg. for checking if we can apply COST on an action.
				//We ignore % modifier in this context.
				int NewValue = AttributeToMod.BaseValue + AttributeToMod.Delta + Mod.GetMagnitude(Effect->GetLevel());
				if (NewValue < 0)
				{
					return false;
				}
			}
		}
	}
	// If all modifiers can be applied without causing negative attribute values, return true
	return true;
}

void UCActionComponent::AddAttributeChangedListener(FGameplayTag AttributeTag, const FAttributeChangedSignature& Event,
                                                    bool bCallNow)
{
	//Create and add new pair to listen for.
	new(AttributeChangeTriggers) TPair<FGameplayTag, FAttributeChangedSignature>(AttributeTag, Event);

	//Convenient for Blueprint that may get bound late and want to get the 'initial' state to apply.
	if (bCallNow)
	{
		FAttribute Attribute;
		//May still be nullptr initally for clients during init.
		if (AttributeSet)
		{
			AttributeSet->GetAttribute(AttributeTag, Attribute);
		}

		FGameplayTagContainer EmptyContainer;

		Event.Execute(this, this, AttributeTag, Attribute.GetValue(), Attribute.GetValue(), EmptyContainer, EAttributeModifierOperation::Invalid);
	}
}

void UCActionComponent::BroadcastAttributeChanged(FGameplayTag InAttributeTag, UCActionComponent* InstigatorComp,
	int InNewValue, int InDelta, FGameplayTagContainer InContextTags, EAttributeModifierOperation ModOperation)
{
	//LOG_INFO("Notify", InAttributeTag.ToString(), "attribute changed");

	//Notify any listeners an attribute changed.
	for (int32 Index = 0; Index < AttributeChangeTriggers.Num(); Index++)
	{
		TPair<FGameplayTag, FAttributeChangedSignature>& SearchPair = AttributeChangeTriggers[Index];
		if (SearchPair.Key.MatchesTag(InAttributeTag))
		{
			//Clients don't have all information available that servers do.
			SearchPair.Value.ExecuteIfBound(this, InstigatorComp, InAttributeTag, InNewValue, InDelta, InContextTags, ModOperation);
		}
	}
}

UCActionComponent* UCActionComponent::GetActionComp(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UCActionComponent>();
	}

	return nullptr;
}

void UCActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//Skip for clients.
	if (!GetOwner()->HasAuthority())
	{
		LOG_WARNING("Client attempting to AddAction");
		return;
	}

	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	check(NewAction);

	NewAction->Initialize(this);

	Actions.Add(NewAction);

	if (ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UCActionComponent::RegisterAction(UCAction* NewAction)
{
	if (!NewAction) return;
	Actions.Add(NewAction);
}

void UCActionComponent::RemoveAction(UCAction* ActionToRemove) //TODO: Maybe impl remove action. 
{
	if (!ensure(ActionToRemove)) //&& !ActionToRemove->IsRunning()). == Checking if theres active timer.
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

UCAction* UCActionComponent::GetAction(TSubclassOf<UCAction> ActionClass) const
{
	
	for (UCAction* Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool UCActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
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

void UCActionComponent::ToggleHighlightOnValidTargetTiles(FGameplayTag itemSlot, ACGridTile* Tile, bool bHighlightOn)
{
	ACUnit* Owner = Cast<ACUnit>(GetOwner());
	FAbility Ability;
	if (!TryGetAbility(itemSlot, Ability) || !Owner)
		return;

	Ability.ToggleHighlightTilesInRange(Tile, bHighlightOn);
}

bool UCActionComponent::IsValidTargetTile(FGameplayTag ItemSlot, ACGridTile* TargetTile)
{
	return GetValidTargetTiles(ItemSlot).Contains(TargetTile);
}

bool UCActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(AttributeSet, *Bunch, *RepFlags);

	for (FAbility& Ability : Abilities)
	{
		for (UCAction* Action : Ability.InstantiatedActions)
		{
			if (Action)
				WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	for (UCAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


void UCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCActionComponent, Abilities);
	DOREPLIFETIME(UCActionComponent, Actions);
	DOREPLIFETIME(UCActionComponent, AttributeSet);
}
