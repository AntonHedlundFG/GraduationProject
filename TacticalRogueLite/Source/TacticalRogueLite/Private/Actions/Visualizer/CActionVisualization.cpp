// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualization.h"
#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Utility\Logging\CLogManager.h"

#include "Actions/CMovementAction.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridTile.h"
#include "CGameState.h"

bool UCActionVisualization::CanVisualizeAction(const UCAction* Action)
{
	if(VisualizedActionTypes.Num() == 0) return true;
	
	for (TSubclassOf<UCAction> ActionType : VisualizedActionTypes)
	{
		if (Action->IsA(ActionType))
			return true;
	}
	
	return false;
}

void UCActionVisualization::Enter()
{
	ReceiveEnter();
}

void UCActionVisualization::Exit()
{
	ReceiveExit();
}

bool UCActionVisualization::Tick(float DeltaTime)
{
	return ReceiveTick(DeltaTime);
}

bool UCActionVisualization::RevertTick(float DeltaTime)
{
	return ReceiveRevertTick(DeltaTime);
}

bool UCActionVisualization::ReceiveCanVisualizeAction_Implementation(UCAction* Action)
{
	return true;
}
bool UCActionVisualization::ReceiveRevertTick_Implementation(float DeltaTime)
{
	LOG_INFO("No undo visualization available for this action");

	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();

	for (ACUnit* Unit : GameState->GetCurrentTurnOrder(true))
	{
		if (Unit->GetTile())
			Unit->SetActorLocation(Unit->GetTile()->GetActorLocation());
	}

	return true;
}

bool UCActionVisualization::ReceiveTick_Implementation(float DeltaTime)
{
	LOG_INFO("No visualization available for this action");

	ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();

	for (ACUnit* Unit : GameState->GetCurrentTurnOrder(true))
	{
		if (Unit->GetTile())
			Unit->SetActorLocation(Unit->GetTile()->GetActorLocation());
	}

	return true;
}

void UCActionVisualization::ReceiveExit_Implementation()
{
}

void UCActionVisualization::ReceiveEnter_Implementation()
{
}
