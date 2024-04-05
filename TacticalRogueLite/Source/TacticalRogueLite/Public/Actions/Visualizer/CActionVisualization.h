// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CActionVisualization.generated.h"

class UCAction;
class UCActionVisualizerSystem;

/**
 * 
 */
UCLASS(Blueprintable)
class TACTICALROGUELITE_API UCActionVisualization : public UObject
{
	GENERATED_BODY()

	friend UCActionVisualizerSystem;

protected:

	//This should return true if the action can be visualized by this effect.
	//It's likely that this function will simply be:
	// return Action->IsA(UCSomeActionImplementation::StaticClass()); 
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanVisualizeAction(UCAction* Action);

	// This will be called by the VisualizerSystem when this visualization is created.
	// Here we can fetch data about what the action did, like which path a movement action used, 
	// or how much damage an attack did.
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void InitializeVisualization(UCAction* FromAction);

	// While the visualization is active within the VisualizerSystem, it will call this function 
	// every tick. It should return true when the visualization is finished, to let the system know
	// it can move on to a new visualization.
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool TickVisualization(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool RevertVisualization(float DeltaTime);

	//Set by the system upon creation.
	UCActionVisualizerSystem* ParentSystem;


	// -- TESTING ----

	float TimePassed;

	// ---------------
};
