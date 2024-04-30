// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_RollItem.h"
#include "CGameInstance.h"
#include "Actions/CActionComponent.h"
#include "GamePlay/Subsystems/CItemRollingSubSystem.h"


void UCAction_RollItem::OnRep_RepData()
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

bool UCAction_RollItem::IsRunning() const
{
	return RepData.bIsRunning;
}

TArray<UCItemData*> UCAction_RollItem::GetItems()
{
	return Items;
}

void UCAction_RollItem::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	Items.Empty(); //?

	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCItemRollingSubSystem* RollingSubSystem = GI->GetSubsystem<UCItemRollingSubSystem>();

	if (RollingSubSystem)
	{
		Items = RollingSubSystem->RollItems(LootTable, GetOwningComponent()->ActiveGameplayTags, Buckets, RollAmount);
		
			//Replicate ItemDatas + "Done"
			//List<Items>.
			//VizualizeAction->GetOwningComponent();
			//Skriv en vizualise, i den-> Enter->UpdateInfo(
			//Widget.UpdateInfo(ItemDatas, ActionComp);


			//Viz- Enter: Open widget
			//Exit: "Close" Widget

			//Click-> PS->EquipSelected(Action,Item)->InventoryComp->EquipItem. "Set Action.Equipped to done"
			//
			
		
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
