

#include "Achievements/VictoryConditions/CVictoryCondition_PickUpKey.h"
#include "CGameMode.h"
#include "Grid/CGridRoom.h"
#include "GridContent/CPickUp.h"

bool UCVictoryCondition_PickUpKey::CheckVictoryCondition()
{
	if (!GameModeRef || !ParentRoom)
		return false;

	if (bWonByCheating) return true;

	for (auto* Key : Keys)
	{
		if (!Key->IsPickedUp())
			return false;
	}

	for (auto* Unit : GameModeRef->GetHeroUnits())
	{
		if (Unit->GetTile() == ParentRoom->GetExitTile())
		{
			return true;
		}
	}

	return false;
}
