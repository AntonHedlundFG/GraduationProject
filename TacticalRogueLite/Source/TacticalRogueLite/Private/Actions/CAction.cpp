
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

bool UCAction::CanStart_Implementation(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	
	return !Comp->ActiveGameplayTags.HasAny(ActionBlockingTags);
}


void UCAction::StartAction_Implementation(AActor* Instigator)
{

	UCActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(ActionTags);

}


void UCAction::StopAction_Implementation(AActor* Instigator)
{

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(ActionTags);

}

void UCAction::UndoAction_Implementation(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(ActionTags);
	bIsUndone = true;
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
	return TArray<ACGridTile*>();
}

bool FAbility::IsValidTargetTile(ACGridTile* fromTile, ACGridTile* toTile)
{
	return GetValidTargetTiles(fromTile).Contains(toTile);
}
