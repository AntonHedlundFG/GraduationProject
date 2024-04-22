// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionVisualizerSystem.generated.h"

class UCActionVisualization;
class UCAction;

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
/// Each class of CAction is likely to have its own CActionVisualization sibling.
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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	//A simple cached reference.
	UPROPERTY()
	TObjectPtr<class ACGameState> GameState;
	
	//Called when GameState->OnActionListUpdate is broadcast.
	UFUNCTION()
	void OnActionListUpdate();

	TQueue<UCActionVisualization*> VisualizationQueue;
	TQueue<UCActionVisualization*> UndoQueue;
	TMap<UCAction*, UCActionVisualization*> ActionToVisualMap;

	//Used to make sure they don't fall out of scope.
	UPROPERTY()
	TArray<UCActionVisualization*> VisualizationList;

	UCActionVisualization* CurrentVisualization;
	bool bCurrentVisualForward;

	int32 ActionListCurrentIndex = -1;
	
	//This should be populated in the editor so both C++ and BP implementations are available.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	TArray<TSubclassOf<UCActionVisualization>> AllAvailableVisualizationTemplates;

	//Here we simply store actual UObject instances of each template, so we can call functions on it.
	UPROPERTY()
	TArray<UCActionVisualization*> AllAvailableVisualizationInstances;

	//This creates a copy of an instanced visualization templates that is able to visualize 
	//the specific action.
	UCActionVisualization* CreateVisualizationForAction(UCAction* Action);

	

		
};
