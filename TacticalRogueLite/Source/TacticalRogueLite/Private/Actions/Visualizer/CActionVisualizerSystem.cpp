// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Actions\Visualizer\CActionVisualization.h"
#include "CGameState.h"
#include "Utility\Logging\CLogManager.h"
#include "Actions\CAction.h"

UCActionVisualizerSystem::UCActionVisualizerSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCActionVisualizerSystem::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ACGameState>(GetOwner());
	if (!GameState)
	{
		LOG_WARNING("Action Visualizer System must be attached to a CGameState to function");
		return;
	}

	for (auto Template : AllAvailableVisualizationTemplates)
	{
		AllAvailableVisualizationInstances.Add(NewObject<UCActionVisualization>(this, Template));
	}

	GameState->OnActionListUpdate.AddDynamic(this, &UCActionVisualizerSystem::OnActionListUpdate);

}

void UCActionVisualizerSystem::EndPlay(EEndPlayReason::Type Reason)
{
	GameState->OnActionListUpdate.RemoveAll(this);
}

void UCActionVisualizerSystem::OnActionListUpdate()
{
	if (GetNetMode() == ENetMode::NM_Client)
		LOG_INFO("Client: ActionListUpdate in VisualizerSystem");
	else
		LOG_INFO("Server: ActionListUpdate in VisualizerSystem");
	if (!CurrentVisualization)
	{
		CurrentVisualization = CreateVisualizationForAction(nullptr);
	}
}

UCActionVisualization* UCActionVisualizerSystem::CreateVisualizationForAction(UCAction* Action)
{
	for (UCActionVisualization* Visualization : AllAvailableVisualizationInstances)
	{
		if (Visualization && Visualization->CanVisualizeAction(Action))
		{
			UCActionVisualization* VisualizationInstance = DuplicateObject<UCActionVisualization>(Visualization, this);
			VisualizationInstance->ParentSystem = this;
			VisualizationInstance->Enter(Action);
			return VisualizationInstance;
		}
	}
	LOG_WARNING("No available visualization template for this type of action");
	return nullptr;
}


void UCActionVisualizerSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentVisualization && CurrentVisualization->Tick(DeltaTime))
	{
		CurrentVisualization = nullptr;
	}
}

