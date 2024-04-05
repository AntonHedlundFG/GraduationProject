// Fill out your copyright notice in the Description page of Project Settings.


#include "CUndoAction.h"
#include "Net/UnrealNetwork.h"

void UCUndoAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCUndoAction, UndoneActions);
}