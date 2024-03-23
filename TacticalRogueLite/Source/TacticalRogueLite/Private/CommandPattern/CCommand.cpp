// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPattern/CCommand.h"
#include "CommandPattern/CConsequence.h"

void UCCommand::StoreConsequence(UCConsequence* inConsequence)
{
	Consequences.Add(inConsequence);
}

void UCCommand::UndoAllConsequences()
{
	while (!Consequences.IsEmpty())
	{
		Consequences.Last()->UndoConsequence();
		Consequences.RemoveAtSwap(Consequences.Num() - 1);
	}
}
