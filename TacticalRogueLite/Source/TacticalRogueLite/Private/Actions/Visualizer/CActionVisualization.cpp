// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualization.h"
#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Utility\Logging\CLogManager.h"

bool UCActionVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return true;
}

void UCActionVisualization::Enter_Implementation(UCAction* FromAction)
{
	TimePassed = 0.0f;
}

void UCActionVisualization::Exit_Implementation(UCAction* FromAction)
{

}

bool UCActionVisualization::Tick_Implementation(float DeltaTime)
{
	
	TimePassed += DeltaTime;
	if (TimePassed > 5.0f)
	{
		if (ParentSystem->GetNetMode() == ENetMode::NM_Client)
			LOG_INFO("Client: We've ticked and finished a visualization");
		else
			LOG_INFO("Server: We've ticked and finished a visualization");
		return true;
	}
	return false;
}

bool UCActionVisualization::RevertTick_Implementation(float DeltaTime)
{
	return true;
}
