// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CTargetableAction.h"

#include "Actions/CActionComponent.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"

void UCTargetableAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCTargetableAction, TargetTile);
}

void UCTargetableAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);
	
	ACGridContent* TargetContent = TargetTile->GetContent();
	if(!TargetContent) return;
	
	UCActionComponent* ActionComponent = TargetContent->GetComponentByClass<UCActionComponent>();
	if (!ActionComponent) return;
	
	for (FAttributeModification& Mod : ModifiersAppliedToTarget)
	{
		Mod.InstigatorComp = ActionComponent;
		int ActualDelta = ActionComponent->ApplyAttributeChange(Mod, 0);
		ModifiersActualDeltas.Add(ActualDelta);
		LOG_INFO("ActualDelta: %d" ,ActualDelta);
	}
}

void UCTargetableAction::UndoAction(AActor* Instigator)
{
	Super::UndoAction(Instigator);

	UCActionComponent* ActionComponent = TargetTile->GetContent()->GetComponentByClass<UCActionComponent>();

	if (ActionComponent)
	{
		for (int i = 0; i < ModifiersAppliedToTarget.Num(); i++)
		{
			FAttributeModification Mod = ModifiersAppliedToTarget[i];
			Mod.InstigatorComp = ActionComponent;
			Mod.bIsUndo = true;
			Mod.Magnitude = -ModifiersActualDeltas[i];
			ActionComponent->ApplyAttributeChange(Mod, 0);
		}
	}
}
