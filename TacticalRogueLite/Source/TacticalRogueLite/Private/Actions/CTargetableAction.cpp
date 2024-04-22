// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CTargetableAction.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"

void UCTargetableAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCTargetableAction, TargetTile);
}