// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAbilityWidget.h"
#include "Items/ItemSlots.h"
#include "Abilities/CAbilityUsageSubsystem.h"

void UCAbilityWidget::InitiateAbilityUse(EItemSlots ItemSlot)
{
	auto* Subsystem = GetWorld()->GetSubsystem<UCAbilityUsageSubsystem>();
	if (Subsystem)
	{
		Subsystem->InitiateAbilityUse(ItemSlot);
	}
}

void UCAbilityWidget::UndoAbility()
{
	auto* Subsystem = GetWorld()->GetSubsystem<UCAbilityUsageSubsystem>();
	if (Subsystem)
	{
		Subsystem->UndoAbility();
	}
}
