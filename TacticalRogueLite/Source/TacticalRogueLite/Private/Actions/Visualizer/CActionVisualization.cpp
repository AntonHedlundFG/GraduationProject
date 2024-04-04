// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualization.h"
#include "Utility\Logging\CLogManager.h"

bool UCActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return true;
}

void UCActionVisualization::InitializeVisualization_Implementation(UCAction* FromAction)
{
	TimePassed = 0.0f;
}

bool UCActionVisualization::TickVisualization_Implementation(float DeltaTime)
{
	TimePassed += DeltaTime;
	if (TimePassed > 5.0f)
	{
		LOG_INFO("We've ticked and finished a visualization");
		return true;
	}
	return false;
}
