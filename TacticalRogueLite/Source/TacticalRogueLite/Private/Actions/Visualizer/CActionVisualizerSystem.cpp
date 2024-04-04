// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions\Visualizer\CActionVisualizerSystem.h"
#include "CGameState.h"
#include "Utility\Logging\CLogManager.h"

// Sets default values for this component's properties
UCActionVisualizerSystem::UCActionVisualizerSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCActionVisualizerSystem::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ACGameState>(GetOwner());
	if (!GameState)
	{
		LOG_WARNING("Action Visualizer System must be attached to a CGameState to function");
		return;
	}

}


// Called every frame
void UCActionVisualizerSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

