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
	UFUNCTION(Category = "Actions")
	virtual bool CanVisualizeAction(UCAction* Action);

	// This will be called by the VisualizerSystem when this visualization is created.
	// Here we can fetch data about what the action did, like which path a movement action used, 
	// or how much damage an attack did.
	UFUNCTION(Category = "Actions")
	virtual void Enter();
	bool bHasBeenEntered = false;

	UFUNCTION(Category = "Actions")
	virtual void Exit();

	// While the visualization is active within the VisualizerSystem, it will call this function 
	// every tick. It should return true when the visualization is finished, to let the system know
	// it can move on to a new visualization.
	UFUNCTION(Category = "Actions")
	virtual bool Tick(float DeltaTime);

	// Works like Tick but should be reversed, for Undoing actions. Note that you might be halfway through
	// the Tick process when this gets called, if the user undoes their action in the midst of the visualization.
	UFUNCTION(Category = "Actions")
	virtual bool RevertTick(float DeltaTime);

	//Set by the system upon creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	UCActionVisualizerSystem* ParentSystem;
	//Set by the system upon creation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	UCAction* VisualizedAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	TArray<TSubclassOf<UCAction>> VisualizedActionTypes;


#pragma region AngelScript events

	// These are necessary as Super::CanStart() cannot be called from AngelScript.
	// It's a known issue, and the suggested workaround is this solution, where we
	// call these Receive-functions from the main functions (CanStart, StartAction, etc)

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "Can Visualize"))
	bool ReceiveCanVisualizeAction(UCAction* Action);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "Enter"))
	void ReceiveEnter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "Exit"))
	void ReceiveExit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "Tick"))
	bool ReceiveTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "RevertTick"))
	bool ReceiveRevertTick(float DeltaTime);

#pragma endregion


};
