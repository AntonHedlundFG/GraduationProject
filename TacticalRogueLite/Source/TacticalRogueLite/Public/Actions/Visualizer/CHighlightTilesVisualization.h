﻿#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CHighlightTilesVisualization.generated.h"

class UCHighlightTileAction;
class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API UCHighlightTilesVisualization : public UCActionVisualization
{
	GENERATED_BODY()
	
	virtual bool CanVisualizeAction_Implementation(UCAction* Action) override;
	virtual void Enter_Implementation() override;
	virtual bool Tick_Implementation(float DeltaTime) override;
	virtual bool RevertTick_Implementation(float DeltaTime) override;

	void ToggleHighlightTilesInRange(bool bHighlightOn);
	
	// Used to determine where in the animation we are.
	float TimePassed = 0.0f;

	// A reference to the cast version of the action
	UPROPERTY()
	UCHighlightTileAction* HighLightAction;
	
};
