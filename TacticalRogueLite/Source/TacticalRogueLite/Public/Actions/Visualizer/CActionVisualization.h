// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CActionVisualization.generated.h"

class UCAction;
class UCActionVisualizerSystem;

/** Each Action that should have a visual effect should have a corresponding actionvisualization
 * which can be used to display the effects visually, without affecting game state.
 * Read the individual BlueprintNativeEvents to see which virtual functions need to be overridden.
 * Note that overriding a BlueprintNativeEvent in c++ requires you to write:
 * virtual bool CanVisualizeAction_Implementation(UCAction* Action) override;
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
	void Enter();
	bool bHasBeenEntered = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void Exit();


	// While the visualization is active within the VisualizerSystem, it will call this function 
	// every tick. It should return true when the visualization is finished, to let the system know
	// it can move on to a new visualization.
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool Tick(float DeltaTime);

	// Works like Tick but should be reversed, for Undoing actions. Note that you might be halfway through
	// the Tick process when this gets called, if the user undoes their action in the midst of the visualization.
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool RevertTick(float DeltaTime);

	//Set by the system upon creation.
	UCActionVisualizerSystem* ParentSystem;
	//Set by the system upon creation.
	UCAction* VisualizedAction;

};
