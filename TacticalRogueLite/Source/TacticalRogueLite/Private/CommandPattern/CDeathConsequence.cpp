// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPattern/CDeathConsequence.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridTile.h"
#include "CGameState.h"

void UCDeathConsequence::ExecuteConsequence()
{
	if (!DyingUnit) return;

	GameState = GetWorld()->GetGameState<ACGameState>();
	if (!GameState) return;

	DeathLocation = DyingUnit->GetActorLocation();
	DyingUnit->SetActorLocation(FVector(10000, 10000, 10000));

	QueueIndex = GameState->TurnOrder.Find(DyingUnit);
	if (QueueIndex >= 0)
		GameState->TurnOrder.RemoveAt(QueueIndex);

	Tile = DyingUnit->GetTile();
	if (Tile)
		Tile->SetContent(nullptr);
}

void UCDeathConsequence::UndoConsequence()
{
	if (!GameState || !DyingUnit) return;

	DyingUnit->SetActorLocation(DeathLocation);

	if (QueueIndex >= 0)
		GameState->TurnOrder.Insert(DyingUnit, QueueIndex);

	if (Tile)
		Tile->SetContent(DyingUnit);
}

FString UCDeathConsequence::ToString()
{
	if (!DyingUnit) return FString("Unknown Unit died.");
	return DyingUnit->GetName() + FString(" died.");
}
