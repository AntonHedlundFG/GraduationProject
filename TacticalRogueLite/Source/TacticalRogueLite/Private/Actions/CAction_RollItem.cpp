// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/CAction_RollItem.h"
#include "CGameInstance.h"
#include "Actions/CActionComponent.h"
#include "Components/Image.h"
#include "GamePlay/Subsystems/CItemRollingSubSystem.h"

class UCGameInstance;


void UCAction_RollItem::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	Items.Empty(); //?

	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	UCItemRollingSubSystem* RollingSubSystem = GI->GetSubsystem<UCItemRollingSubSystem>();

	if (RollingSubSystem)
	{
		Items = RollingSubSystem->RollItems(LootTable, GetOwningComponent()->ActiveGameplayTags, Buckets, RollAmount);
		
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
