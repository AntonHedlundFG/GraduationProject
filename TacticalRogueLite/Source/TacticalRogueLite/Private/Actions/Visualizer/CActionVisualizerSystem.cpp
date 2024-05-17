// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "Actions\Visualizer\CActionVisualization.h"
#include "CGameState.h"
#include "Utility\Logging\CLogManager.h"
#include "Actions\CAction.h"
#include "CUndoAction.h"

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

	//Add the default implementation last as a catch-all at the top
	AllAvailableVisualizationInstances.Add(NewObject<UCActionVisualization>(this, UCActionVisualization::StaticClass()));
	
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
	TArray<UCAction*> ActionList = GameState->ActionHistory;
	ActionList.Append(GameState->ActionList);
	if (ActionList.IsEmpty())
		return;

	while (++ActionListCurrentIndex < ActionList.Num() && IsValid(ActionList[ActionListCurrentIndex]))
	{
		if (UCUndoAction* Undo = Cast<UCUndoAction>(ActionList[ActionListCurrentIndex]))
		{
			for (TSoftObjectPtr<UCAction> Action : Undo->UndoneActions)
			{
				if (!ActionToVisualMap.Contains(Action.Get())) continue;

				TObjectPtr<UCActionVisualization> Visual = ActionToVisualMap[Action.Get()];
				if (IsValid(Visual))
					UndoQueue.Enqueue(Visual);
			}
			continue;
		}

		//Create a visualization for a regular action.
		UCActionVisualization* Visual = CreateVisualizationForAction(ActionList[ActionListCurrentIndex]);
		if (Visual)
		{
			VisualizationQueue.Enqueue(Visual);
			ActionToVisualMap.Add(ActionList[ActionListCurrentIndex], Visual);
			VisualizationList.Add(Visual);
		}
	}

	ActionListCurrentIndex--;
	return;
}

UCActionVisualization* UCActionVisualizerSystem::CreateVisualizationForAction(UCAction* Action)
{
	for (int i = AllAvailableVisualizationInstances.Num() - 1; i >= 0; --i)
	{
		UCActionVisualization* Visualization = AllAvailableVisualizationInstances[i];
		if (Visualization && Visualization->CanVisualizeAction(Action))
		{
			UCActionVisualization* VisualizationInstance = DuplicateObject<UCActionVisualization>(Visualization, this);
			VisualizationInstance->ParentSystem = this;
			VisualizationInstance->VisualizedAction = Action;
			return VisualizationInstance;
		}
	}

	LOG_WARNING("No available visualization template for this type of action");
	return nullptr;
}


void UCActionVisualizerSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SafetyDelay < StartAfterSafetyDelay)
	{
		SafetyDelay += DeltaTime;
		return;
	}

	if (IsValid(CurrentVisualization))
	{
		if (!CurrentVisualization->bHasBeenEntered)
		{
			LOG_INFO("Now entering visualiation: %s", *CurrentVisualization->GetName());
			CurrentVisualization->Enter();
			CurrentVisualization->bHasBeenEntered = true;
		}

		if (bCurrentVisualForward && !CurrentVisualization->VisualizedAction->bIsUndone)
		{
			if (CurrentVisualization->Tick(DeltaTime * GameState->GetGameSpeed()))
			{
				LOG_INFO("Now exiting visualiation: %s", *CurrentVisualization->GetName());
				CurrentVisualization = nullptr;
			}
		}
		else
		{
			if (CurrentVisualization->RevertTick(DeltaTime * GameState->GetGameSpeed()))
			{
				LOG_INFO("Now exiting undo visualiation: %s", *CurrentVisualization->GetName());
				CurrentVisualization = nullptr;
			}
		}
	}

	if (IsValid(CurrentVisualization))
		return;

	if (!UndoQueue.IsEmpty())
	{
		UndoQueue.Dequeue(CurrentVisualization);
		bCurrentVisualForward = false;
		bShouldBroadcastVisualizationComplete = true;
		return;
	}

	if (!VisualizationQueue.IsEmpty())
	{
		VisualizationQueue.Dequeue(CurrentVisualization);
		bCurrentVisualForward = !CurrentVisualization->VisualizedAction->bIsUndone;
		bShouldBroadcastVisualizationComplete = true;
		return;
	}

	//Only broadcast the first frame after visualization is complete. Then wait for a new visualization to happen.
	if (bShouldBroadcastVisualizationComplete)
	{
		OnVisualizationComplete.Broadcast();
		bShouldBroadcastVisualizationComplete = false;
	}
}

