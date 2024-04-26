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
	
	ToggleHighlightTilesInRange(HighLightAction->TargetTile, true);
}

bool UCHighlightTilesVisualization::Tick_Implementation(float DeltaTime)
{
	// Updates the current time frame of the animation
	TimePassed = TimePassed + DeltaTime;

	// If the time passed is greater than the duration, we're done.
	if(TimePassed >= Duration)
	{
		// Remove the highlight from the tiles
		ToggleHighlightTilesInRange(HighLightAction->TargetTile, false);
		
		return true;
	}
	
	return false;
}


bool UCHighlightTilesVisualization::RevertTick_Implementation(float DeltaTime)
{
	// No revert functionality for this visualization.
	return true;
}

void UCHighlightTilesVisualization::ToggleHighlightTilesInRange(ACGridTile* fromTile, bool bHighlightOn)
{
	FAbility& Ability = HighLightAction->GetAbility();

	// Loop over all Actions in the ability and toggle them with their respective highlightmode
	for (auto Action : Ability.InstantiatedActions)
	{
		UCTargetableAction* TargetableAction = Cast<UCTargetableAction>(Action);
		if (TargetableAction)
		{
			auto Tiles = TargetableAction->GetValidTargetTiles(fromTile);

			for (const ACGridTile* Tile : Tiles)
			{
				const ETileHighlightModes HighlightMode = TargetableAction->GetHighlightMode();
				if(bHighlightOn)
				{
					Tile->GetHighlightComponent()->AppendHighlightMode(HighlightMode);
				}
				else{
					Tile->GetHighlightComponent()->RemoveHighlightMode(HighlightMode);
				}
			}
		}
	}
}
