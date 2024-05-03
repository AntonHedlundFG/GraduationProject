

#include "Achievements/VictoryConditions/CVictoryCondition_KillEnemies.h"
#include "CGameMode.h"
#include "Actions/CActionComponent.h"

bool UCVictoryCondition_KillEnemies::CheckVictoryCondition()
{
	if (!GameModeRef)
		return false;

	if (bWonByCheating) return true;
	
	FAttribute Health;
	FGameplayTag HealthTag = FGameplayTag::RequestGameplayTag(FName("Attribute.Health"));
	for (auto* Unit : Enemies)
	{
		if (Unit->GetActionComp()->GetAttribute(HealthTag, Health))
		{
			if( Health.BaseValue > 0)
			{
				return false;
			}
		}
	}
	return true;
}
