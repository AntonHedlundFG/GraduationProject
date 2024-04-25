#include "AI/CHighlightTileAction.h"
#include "Grid/CGridTile.h"

TArray<ACGridTile*> UCHighlightTileAction::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
	if(IsValid(HighlightedAbility))
	{
		return HighlightedAbility.GetValidTargetTiles(inTile);
	}
	return TArray<ACGridTile*>();
}

