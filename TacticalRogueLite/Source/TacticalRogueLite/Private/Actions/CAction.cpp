
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Actions/CTargetableAction.h"
//TODO: include project h.file
#include "Net/UnrealNetwork.h"



//"Inline the generated cpp file into module cpp file to improve compile time because less header parsing is required."
#include UE_INLINE_GENERATED_CPP_BY_NAME(CAction)

void UCAction::OnRep_bIsUndone()
{
	LOG_INFO("Action undone");
}

void UCAction::Initialize(UCActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
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

	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())
		Comp->ActiveGameplayTags.AppendTags(ActionTags);

	ReceiveStartAction(Instigator);
}


void UCAction::StopAction(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())
		Comp->ActiveGameplayTags.RemoveTags(ActionTags);

	ReceiveStopAction(Instigator);
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

TSet<ACGridTile*> UCAction::GetActionInfluencedTiles_Implementation(ACGridTile* fromTile)
{
	return { fromTile }; // Default implementation, just return the tile the action was started from.
}

void UCAction::UndoAction(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp) && !ActionTags.IsEmpty())
		Comp->ActiveGameplayTags.RemoveTags(ActionTags);

	ReceiveUndoAction(Instigator);

	bIsUndone = true;
	if (IsValid(Comp))
		GetOwningComponent()->OnActionUndo.Broadcast(GetOwningComponent(), this);
}

UWorld* UCAction::GetWorld() const
{
	if (GetOwningComponent())
		return GetOwningComponent()->GetWorld();

	if (AActor* Actor = Cast<AActor>(GetOuter()))
		return Actor->GetWorld();

	return nullptr;
}


void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCAction, ActionComp);
	DOREPLIFETIME(UCAction, bIsUndone);
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
