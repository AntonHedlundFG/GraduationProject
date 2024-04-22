

#include "Achievements/VictoryConditions/CVictoryCondition_KillEnemies.h"

#include "CGameMode.h"
#include "Attributes/CAttributeComponent.h"

bool UCVictoryCondition_KillEnemies::CheckVictoryCondition()
{
	if (!GameModeRef)
		return false;

	if (bWonByCheating) return true;

	for (auto* Unit : Enemies)
	{
		UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(Unit);
		if (Attributes->GetHealth() > 0)
			return false;
	}
	return true;
}
