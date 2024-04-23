// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualization.h"
#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Utility\Logging\CLogManager.h"

#include "Actions/CMovementAction.h"
#include "GridContent/CUnit.h"
#include "Grid/CGridTile.h"

bool UCActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return true;
}

void UCActionVisualization::Enter_Implementation()
{
}

void UCActionVisualization::Exit_Implementation()
{

}

bool UCActionVisualization::Tick_Implementation(float DeltaTime)
{
	LOG_INFO("No visualization available for this action");

	return true;
}

bool UCActionVisualization::RevertTick_Implementation(float DeltaTime)
{
	LOG_INFO("No undo visualization available for this action");
	return true;
}
