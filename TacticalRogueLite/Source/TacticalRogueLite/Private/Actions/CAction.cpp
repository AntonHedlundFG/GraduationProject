
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "Grid/CGridTile.h"
#include "Actions/CTargetableAction.h"
#include "Net/UnrealNetwork.h"
#include "Grid/CTileHighlightComponent.h"


//"Inline the generated cpp file into module cpp file to improve compile time because less header parsing is required."
#include "Utility/Logging/CLogManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CAction)

void UCAction::OnRep_bIsUndone()
{
	LOG_INFO("Action undone");
}

void UCAction::Initialize(UCActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	
	if (ActionComp)
	{
		ActionComp->RegisterAction(this);
	}
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return ActionComp;
}

bool UCAction::CanStart(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	
	return (!Comp->ActiveGameplayTags.HasAny(ActionBlockingTags) && ReceiveCanStart(Instigator));
}


void UCAction::StartAction(AActor* Instigator)
{
	for (FAttributeModification& Mod : ModifiersAppliedToOwner)
	{
		int ActualDelta = GetOwningComponent()->ApplyAttributeChange(Mod, 0);
		ModifiersActualDeltas.Add(ActualDelta);
	}
	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())

	ReceiveStartAction(Instigator);
}


void UCAction::StopAction(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())

	ReceiveStopAction(Instigator);
}

TSet<ACGridTile*> UCAction::GetActionInfluencedTiles_Implementation(ACGridTile* fromTile)
{
	return { fromTile }; // Default implementation, just return the tile the action was started from.
}

void UCAction::UndoAction(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())

	ReceiveUndoAction(Instigator);

	for (int i = 0; i < ModifiersAppliedToOwner.Num(); i++)
	{
		FAttributeModification Mod = ModifiersAppliedToOwner[i];
		Mod.bIsUndo = true;
		Mod.Magnitude = -ModifiersActualDeltas[i];
		GetOwningComponent()->ApplyAttributeChange(Mod, 0);
	}

	bIsUndone = true;
	if (IsValid(Comp))
		GetOwningComponent()->OnActionUndo.Broadcast(GetOwningComponent(), this);
}

bool UCAction::ReceiveCanStart_Implementation(AActor* Instigator)
{
	return true;
}

void UCAction::ReceiveStartAction_Implementation(AActor* Instigator)
{
}

void UCAction::ReceiveUndoAction_Implementation(AActor* Instigator)
{
}

void UCAction::ReceiveStopAction_Implementation(AActor* Instigator)
{
}

void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCAction, ActionComp);
	DOREPLIFETIME(UCAction, bIsUndone);
}

void UCAction::ToggleHighlightTilesInRange(FAbility Ability, ACGridTile* fromTile, bool bHighlightOn)
{
	if (IsValid(Ability))
		Ability.ToggleHighlightTilesInRange(fromTile, bHighlightOn);
}

UWorld* UCAction::GetWorld() const
{
	if (GetOwningComponent())
		return GetOwningComponent()->GetWorld();

	if (AActor* Actor = Cast<AActor>(GetOuter()))
		return Actor->GetWorld();

	return nullptr;
}

TSet<ACGridTile*> FAbility::GetInfluencedTiles(ACGridTile* fromTile)
{
	TSet <ACGridTile*> InfluencedTiles;
	for (auto Action : InstantiatedActions)
	{
		InfluencedTiles.Append(Action->GetActionInfluencedTiles(fromTile));
	}
	return InfluencedTiles;
}

TArray<ACGridTile*> FAbility::GetValidTargetTiles(ACGridTile* fromTile)
{
	for (auto Action : InstantiatedActions)
	{
		UCTargetableAction* Targetable = Cast<UCTargetableAction>(Action);
		if (Targetable)
		{
			return Targetable->GetValidTargetTiles(fromTile);
		}
	}
	//If no items are targetable, return yourself as a target so the ability activates automatically.
	return TArray<ACGridTile*> { fromTile };
}

bool FAbility::IsValidTargetTile(ACGridTile* fromTile, ACGridTile* toTile)
{
	return GetValidTargetTiles(fromTile).Contains(toTile);
}

void FAbility::ToggleHighlightTilesInRange(ACGridTile* fromTile, bool bHighlightOn)
{
	for (const auto Action : InstantiatedActions)
	{
		UCTargetableAction* TargetableAction = Cast<UCTargetableAction>(Action);
		if (TargetableAction)
		{
			auto Tiles = TargetableAction->GetValidTargetTiles(fromTile);

			for (const ACGridTile* Tile : Tiles)
			{
				const ETileHighlightModes HighlightMode = TargetableAction->GetHighlightMode();
				if(bHighlightOn)
				{
					Tile->GetHighlightComponent()->AppendHighlightMode(HighlightMode);
				}
				else{
					Tile->GetHighlightComponent()->RemoveHighlightMode(HighlightMode);
				}
			}
		}
	}
}
