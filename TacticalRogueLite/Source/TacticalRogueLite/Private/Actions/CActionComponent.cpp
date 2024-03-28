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

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UCAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UCActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop all
	// TArray<UCAction*> ActionsCopy = Actions;
	// for (UCAction* Action : ActionsCopy)
	// {
	// 	if (Action->IsRunning())
	// 	{
	// 		Action->StopAction(GetOwner());
	// 	}
	// }

	Super::EndPlay(EndPlayReason);
}

//Disabled, just enable for debugging purposes.
void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	// 	for (USAction* Action : Actions)
	// 	{
	// 		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	// 
	// 		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	// 	}
}
//
void UCActionComponent::AddAbility(FAbility Ability)
{
	RemoveAbility(Ability.InventorySlotTag);
	for (TSubclassOf<UCAction> ActionClass : Ability.Actions)
	{
		UCAction* InstancedObject = NewObject<UCAction>(GetOwner(), ActionClass);
		InstancedObject->Initialize(this);
		Ability.InstantiatedActions.Add(InstancedObject);
	}
	Abilities.Add(Ability);
}

void UCActionComponent::RemoveAbility(FGameplayTag ItemSlot)
{
	for (FAbility& Ability : Abilities)
	{
		if (Ability.InventorySlotTag == ItemSlot)
		{
			Abilities.Remove(Ability);
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

void UCActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass)
{
	if (!ensure(ActionClass))//Macro that is non-fatal, execution is allowed to continue but will break into the debugger if false.
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		//UE_LOGFMT(LogGame, Warning, "Client attempting to AddAction. [Class: {Class}]", GetNameSafe(ActionClass));
		return;
	}

	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	check(NewAction); //Macro that is treated as fatal error if false.

	NewAction->Initialize(this); //TODO: Add

	Actions.Add(NewAction);

	if (NewAction->IsAutoStart() && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UCActionComponent::RemoveAction(UCAction* ActionToRemove)
{
	// if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	// {
	// 	return;
	// }

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


/*bool UCActionComponent::StartActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	//SCOPE_CYCLE_COUNTER(STAT_StartActionByName); //TODO: Enable stat commands/ask group if I can use them. 
	// Inline variant, convenient when only used once in code, visible in Viewport stats. https://www.tomlooman.com/unreal-engine-profiling-stat-commands/
	//DECLARE_SCOPE_CYCLE_COUNTER(TEXT("StartActionByName"), StartActionByName, STATGROUP_ROGUELITE);

	// Visible in Unreal Insights
	//SCOPED_NAMED_EVENT(StartActionName, FColor::Green);

	/*for (UCAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			// Bookmark for Unreal Insights https://dev.epicgames.com/documentation/en-us/unreal-engine/trace-in-unreal-engine-5
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			{
				// Scoped within the curly braces. the _FSTRING variant adds additional tracing overhead due to grabbing the class name every time
				//SCOPED_NAMED_EVENT_FSTRING(Action->GetClass()->GetName(), FColor::White);

				Action->StartAction(Instigator);
			}

			return true;
		}
	}

	return false;#1#
}*/


/*bool UCActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{
			if (Action->IsRunning())
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}*/


void UCActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	//StartActionByName(Instigator, ActionName);
}


void UCActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	//StopActionByName(Instigator, ActionName);
}

bool UCActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
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

	DOREPLIFETIME(UCActionComponent, Actions);
	DOREPLIFETIME(UCActionComponent, Abilities);
}
