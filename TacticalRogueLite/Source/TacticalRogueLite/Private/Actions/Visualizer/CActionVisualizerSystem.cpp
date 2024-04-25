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

	for (auto Template : AllAvailableVisualizationTemplates)
	{
		AllAvailableVisualizationInstances.Add(NewObject<UCActionVisualization>(this, Template));
	}
	//Add the default implementation last as a catch-all
	AllAvailableVisualizationInstances.Add(NewObject<UCActionVisualization>(this, UCActionVisualization::StaticClass()));

	GameState->OnActionListUpdate.AddDynamic(this, &UCActionVisualizerSystem::OnActionListUpdate);

}

void UCActionVisualizerSystem::EndPlay(EEndPlayReason::Type Reason)
{
	GameState->OnActionListUpdate.RemoveAll(this);
}

void UCActionVisualizerSystem::OnActionListUpdate()
{	

	if (GameState->ActionList.IsEmpty())
	{
		ActionListCurrentIndex = -1;
		//ActionToVisualMap.Empty();
		return;
	}

	while (++ActionListCurrentIndex < GameState->ActionList.Num() && GameState->ActionList[ActionListCurrentIndex])
	{
		//Handle undo actions differently since they have to undo other visualizations
		if (UCUndoAction* Undo = Cast<UCUndoAction>(GameState->ActionList[ActionListCurrentIndex]))
		{
			for (TSoftObjectPtr<UCAction> Action : Undo->UndoneActions)
			{
				TObjectPtr<UCActionVisualization> Visual = ActionToVisualMap[Action.Get()];
				if (IsValid(Visual))
					UndoQueue.Enqueue(Visual);
			}
			continue;
		}

		//Create a visualization for a regular action.
		UCActionVisualization* Visual = CreateVisualizationForAction(GameState->ActionList[ActionListCurrentIndex]);
		if (Visual)
		{
			VisualizationQueue.Enqueue(Visual);
			ActionToVisualMap.Add(GameState->ActionList[ActionListCurrentIndex], Visual);
			VisualizationList.Add(Visual);
		}
		
	}
	//Counteract last ++
	ActionListCurrentIndex--;


}

UCActionVisualization* UCActionVisualizerSystem::CreateVisualizationForAction(UCAction* Action)
{
	for (UCActionVisualization* Visualization : AllAvailableVisualizationInstances)
	{
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
			CurrentVisualization->Enter();
			CurrentVisualization->bHasBeenEntered = true;
		}

		if (bCurrentVisualForward && !CurrentVisualization->VisualizedAction->bIsUndone)
		{
			if (CurrentVisualization->Tick(DeltaTime))
				CurrentVisualization = nullptr;
		}
		else
		{
			if (CurrentVisualization->RevertTick(DeltaTime))
				CurrentVisualization = nullptr;
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

