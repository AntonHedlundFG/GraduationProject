#include "Actions/Visualizer/CHighlightTilesVisualization.h"
#include "AI/CHighlightTileAction.h"
#include "Grid/CGridTile.h"
#include "Grid/CTileHighlightComponent.h"

void UCHighlightTilesVisualization::Enter()
{
	TimePassed = 0.0f;
	HighLightAction = Cast<UCHighlightTileAction>(VisualizedAction);
	
	ToggleHighlightTilesInRange(true);
}

bool UCHighlightTilesVisualization::Tick(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = TimePassed + DeltaTime;

	if(HighLightAction->GetDuration() - TimePassed < 0.1f)
	{
		HighLightAction->TargetTile->GetHighlightComponent()->AppendHighlightMode(ETileHighlightModes::ETHM_Hovered);
	}

	// If the time passed is greater than the duration, we're done.
	if(TimePassed >= HighLightAction->GetDuration())
	{
		// Remove the highlight from the tiles
		ToggleHighlightTilesInRange(false);
		HighLightAction->TargetTile->GetHighlightComponent()->RemoveHighlightMode(ETileHighlightModes::ETHM_Hovered);
		return true;
	}
	
	return false;
}


bool UCHighlightTilesVisualization::RevertTick(float DeltaTime)
{
	// No revert functionality for this visualization.
	return true;
}

void UCHighlightTilesVisualization::ToggleHighlightTilesInRange(bool bHighlightOn)
{
	if(!HighLightAction) return;
	
	TArray<ACGridTile*>& Tiles = HighLightAction->GetAffectedTiles();
	if(Tiles.IsEmpty()) return;
	
	TArray<ETileHighlightModes>& HighlightModes = HighLightAction->GetHighlightModes();
	
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		const ETileHighlightModes HighlightMode = HighlightModes[i];
		const ACGridTile* Tile = Tiles[i];
		if(bHighlightOn)
		{
			Tile->GetHighlightComponent()->AppendHighlightMode(HighlightMode);
		}
		else{
			Tile->GetHighlightComponent()->RemoveHighlightMode(HighlightMode);
		}
	}
}
