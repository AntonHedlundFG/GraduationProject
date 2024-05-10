

#include "Achievements/VictoryConditions/CVictoryCondition_PickUpKey.h"
#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Actions/CDeathAction.h"
#include "Grid/CGridRoom.h"
#include "GridContent/CPickUp.h"
#include "GridContent/CUnit.h"

UCVictoryCondition_PickUpKey::UCVictoryCondition_PickUpKey()
{
	ConditionText = "Grab all the keys and move to the exit!";
}

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
			for (auto Enemy : Enemies)
			{
				UCDeathAction* DeathAction = NewObject<UCDeathAction>(Enemy, UCDeathAction::StaticClass());
				DeathAction->AffectedUnit = Enemy;
				
				GameModeRef->RegisterAction(DeathAction);
			}
			
			return true;
		}
	}

	return false;
}
