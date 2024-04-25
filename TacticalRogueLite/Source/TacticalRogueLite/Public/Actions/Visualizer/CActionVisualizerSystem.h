// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionVisualizerSystem.generated.h"

class UCActionVisualization;
class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisualizationComplete);

/// <summary>
/// 
/// This is a component that needs to be attached to CGameState specifically.
/// 
/// It listens to changes in the replicated ActionList and ActionHistory of the GameState
/// and instantiates entirely local CActionVisualizations for each Action.
/// These visualizations are meant to be entirely visual representations of actions, for example
/// physically moving a pawn across the board, playing an animation, spawning a particle effect, 
/// or even animating UI elements.
/// 
/// Each class of CAction is likely to have its own CActionVisualization sibling, however it is possible
/// to make "default" implementations, such as a generic projectile visualization for any projectile ability
/// that has no specific visualization yet. Read the comment on AllAvailableVisualizationTemplates.
/// 
/// The idea is:
/// CActions are managed by the server and replicated to each client.
/// CActionVisualizations are managed individually by each client and are NOT replicated.
/// 
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALROGUELITE_API UCActionVisualizerSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionVisualizerSystem();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Triggers the first frame after all visualizations are finished.
	//Not after each visualization.
	FOnVisualizationComplete OnVisualizationComplete;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	bool bShouldBroadcastVisualizationComplete = false;

	//A simple cached reference.
	UPROPERTY()
	TObjectPtr<class ACGameState> GameState;
	
	//Called when GameState->OnActionListUpdate is broadcast.
	UFUNCTION()
	void OnActionListUpdate();

	//Queue of visualizations that are created but have not yet been visualized.
	TQueue<UCActionVisualization*> VisualizationQueue;
	//Queue of past visualizations that are finished but whose actions have been undone.
	TQueue<UCActionVisualization*> UndoQueue;

	//Lookup map so that we can know which visualizations to undo when its action is undone.
	TMap<UCAction*, UCActionVisualization*> ActionToVisualMap;

	//We have to make sure visualization objects don't fall out of scope, so we keep them referenced here.
	//If we need to clear the history for memory purposes, we can clean this array up at regular intervals.
	UPROPERTY()
	TArray<UCActionVisualization*> VisualizationList;

	//This is the currently visualized object, which is retrieved from either the VisualizationQueue or UndoQueue
	UCActionVisualization* CurrentVisualization;
	//Are we visualizing this forwards, or backwards (do or undo)
	bool bCurrentVisualForward;

	//An index tracker so that we know which action to visualize, and maintain order.
	int32 ActionListCurrentIndex = -1;
	
	/// <summary>
	/// This should be populated in the editor so both C++ and BP implementations are available.
	/// It is a list of all available classes of ActionVisualizations. The order matters; 
	/// as the first object that returns CanVisualizeAction() for an action will be chosen as the template.
	/// This means a generic visualization should be at the end of the list, where a specific one should be at the top.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	TArray<TSubclassOf<UCActionVisualization>> AllAvailableVisualizationTemplates;

	//Here we simply store actual UObject instances of each template, so we can call functions on it.
	//The reason we don't use a static function call is that it isn't supported by AngelScript/Blueprints.
	UPROPERTY()
	TArray<UCActionVisualization*> AllAvailableVisualizationInstances;

	//This creates a copy of an instanced visualization templates that is able to visualize 
	//the specific action.
	UCActionVisualization* CreateVisualizationForAction(UCAction* Action);

	//We use this just to make sure we don't start visualizing anything before
	//everything is initialized properly.
	float StartAfterSafetyDelay = 1.0f;
	float SafetyDelay = 0.0f;

		
};
