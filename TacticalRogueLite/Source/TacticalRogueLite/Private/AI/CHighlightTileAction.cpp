#include "AI/CHighlightTileAction.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"

TArray<ACGridTile*> UCHighlightTileAction::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
	if(IsValid(AbilityToHighlight))
	{
		return AbilityToHighlight.GetValidTargetTiles(inTile);
	}
	return TArray<ACGridTile*>();
}

void UCHighlightTileAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCHighlightTileAction, AbilityToHighlight);
}