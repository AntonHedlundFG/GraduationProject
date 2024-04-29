// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_RollItem.h"
#include "CGameInstance.h"
#include "Actions/CActionComponent.h"
#include "GamePlay/Subsystems/CItemRollingSubSystem.h"


void UCAction_RollItem::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCItemRollingSubSystem* RollingSubSystem = GI->GetSubsystem<UCItemRollingSubSystem>();

	if (RollingSubSystem)
	{
		if(RollingSubSystem->RollItems(LootTable, GetOwningComponent()->ActiveGameplayTags, Buckets, RollAmount))
		{
			//StopAction(GetOuter); ? Stop necessary?
		}
	}
	
}

void UCAction_RollItem::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);
}

void UCAction_RollItem::UndoAction(AActor* Instigator)
{
	Super::UndoAction(Instigator);
}
