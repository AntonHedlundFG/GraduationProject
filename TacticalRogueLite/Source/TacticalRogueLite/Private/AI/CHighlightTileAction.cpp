#include "AI/CHighlightTileAction.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"

void UCHighlightTileAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);
	
	// Loop over all Actions in the ability and toggle them with their respective highlightmode
	for (auto Action : AbilityToHighlight.InstantiatedActions)
	{
		UCTargetableAction* TargetableAction = Cast<UCTargetableAction>(Action);
		if (TargetableAction)
		{
			auto Tiles = TargetableAction->GetValidTargetTiles(FromTile);

			for (ACGridTile* Tile : Tiles)
			{
				AffectedTiles.Add(Tile);
				HighlightModes.Add(TargetableAction->GetTargetHighlightMode());
			}
		}
	}
}

void UCHighlightTileAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCHighlightTileAction, AbilityToHighlight);
	DOREPLIFETIME(UCHighlightTileAction, FromTile);
	DOREPLIFETIME(UCHighlightTileAction, HighlightDuration);
	DOREPLIFETIME(UCHighlightTileAction, AffectedTiles);
	DOREPLIFETIME(UCHighlightTileAction, HighlightModes);
}