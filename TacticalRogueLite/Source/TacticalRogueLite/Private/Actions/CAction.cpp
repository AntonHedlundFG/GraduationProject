
#include "Actions/CAction.h"
#include "Actions/CActionComponent.h"
#include "GridContent/CUnit.h"
#include "Actions/CTargetableAction.h"
//TODO: include project h.file
#include "Net/UnrealNetwork.h"



//"Inline the generated cpp file into module cpp file to improve compile time because less header parsing is required."
#include UE_INLINE_GENERATED_CPP_BY_NAME(CAction)

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
	// if (IsRunning())
	// {
	// 	return false;
	// }

	UCActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}


void UCAction::StartAction_Implementation(AActor* Instigator) //TODO: call it execute. 
{
	//UE_LOGFMT(LogGame, Log, "Started: {ActionName}", GetName());

	UCActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	// RepData.bIsRunning = true;
	// RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		//TimeStarted = GetWorld()->TimeSeconds; 
	}

	//GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UCAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOGFMT(LogGame, Log, "Stopped: {name}", GetName());

	//ensureAlways(bIsRunning);

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	// RepData.bIsRunning = false;
	// RepData.Instigator = Instigator;

	//GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

void UCAction::UndoAction_Implementation(AActor* Instigator)
{
	//TODO:
}

UWorld* UCAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>.
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
/*
void UCAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool UCAction::IsRunning() const
{
	return RepData.bIsRunning;
}
*/


void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCAction, TimeStarted);
	DOREPLIFETIME(UCAction, ActionComp);
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
