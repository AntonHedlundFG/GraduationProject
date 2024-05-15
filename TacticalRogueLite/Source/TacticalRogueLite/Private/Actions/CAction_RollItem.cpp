// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_RollItem.h"
#include "CGameInstance.h"
#include "Actions/CActionComponent.h"
#include "GamePlay/Subsystems/CItemRollingSubSystem.h"
#include "Net/UnrealNetwork.h"


// void UCAction_RollItem::OnRep_RepData() //Override? 
// {
// 	if (!RepData.bIsRunning)
// 	{
// 		StopAction(RepData.Instigator);
// 	}
// }

TArray<UCItemData*> UCAction_RollItem::GetItems()
{
	return Items;
}

void UCAction_RollItem::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	// RepData.bIsRunning = true;
	// RepData.Instigator = Instigator;

	
	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCItemRollingSubSystem* RollingSubSystem = GI->GetSubsystem<UCItemRollingSubSystem>();

	if (RollingSubSystem)
	{
		TArray<UCItemData*> ItemDatas;
		ItemDatas.Append(RollingSubSystem->RollItems(LootTable, GetOwningComponent()->ActiveGameplayTags, Buckets, RollAmount,ERollType::WithoutReplacement));
		ItemDatas.Append(RollingSubSystem->RollItems(LootTable, GetOwningComponent()->ActiveGameplayTags, {CharmBucket}, 2,ERollType::WithoutReplacement)); //Charms.
		Items = ItemDatas;
	}
}

void UCAction_RollItem::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);
	
	// RepData.bIsRunning = false;
}

void UCAction_RollItem::UndoAction(AActor* Instigator)
{
	Super::UndoAction(Instigator);
}

void UCAction_RollItem::OnRep_RepData()
{
	if (!RepData.bIsRunning)
 	{
 		StopAction(RepData.Instigator);
 	}
}

void UCAction_RollItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UCAction_RollItem, RepData);
	DOREPLIFETIME(UCAction_RollItem, Items);
}
