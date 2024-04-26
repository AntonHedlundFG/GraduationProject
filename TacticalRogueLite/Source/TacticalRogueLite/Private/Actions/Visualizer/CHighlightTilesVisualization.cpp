#include "Actions/Visualizer/CHighlightTilesVisualization.h"

#include "AI/CHighlightTileAction.h"
#include "Grid/CGridTile.h"
#include "Grid/CTileHighlightComponent.h"

bool UCHighlightTilesVisualization::CanVisualizeAction_Implementation(UCAction* Action)
{
	return Action->IsA(UCHighlightTileAction::StaticClass());
}

void UCHighlightTilesVisualization::Enter_Implementation()
{
	TimePassed = 0.0f;
	HighLightAction = Cast<UCHighlightTileAction>(VisualizedAction);

	HighlightedTiles = HighLightAction->GetValidTargetTiles(HighLightAction->TargetTile);

	for (ACGridTile* HighlightedTile : HighlightedTiles)
	{
		HighlightedTile->GetHighlightComponent()->AppendHighlightMode(HighLightAction->GetHighlightMode());
	}
}

bool UCHighlightTilesVisualization::Tick_Implementation(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = TimePassed + DeltaTime;

	// If the time passed is greater than the duration, we're done.
	if(TimePassed >= Duration)
	{
		// Remove the highlight from the tiles
		for (ACGridTile* HighlightedTile : HighlightedTiles)
		{
			HighlightedTile->GetHighlightComponent()->RemoveHighlightMode(HighLightAction->GetHighlightMode());
		}
		
		return true;
	}
	
	return false;
}


bool UCHighlightTilesVisualization::RevertTick_Implementation(float DeltaTime)
{
	// No revert functionality for this visualization.
	return true;
}