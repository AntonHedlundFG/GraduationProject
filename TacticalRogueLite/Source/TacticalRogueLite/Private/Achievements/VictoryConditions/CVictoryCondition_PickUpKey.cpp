

#include "Achievements/VictoryConditions/CVictoryCondition_PickUpKey.h"
#include "CGameMode.h"
#include "GridContent/CPickUp.h"

bool UCVictoryCondition_PickUpKey::CheckVictoryCondition()
{
	if (!GameModeRef)
		return false;

	if (bWonByCheating) return true;

	for (auto* Key : Keys)
	{
		if (!Key->IsPickedUp())
			return false;
	}

	return true;
}
