#include "Utility/SaveGame/CSaveGame.h"

UCSaveGame::UCSaveGame()
{
	
	SavedRandomStream = FRandomStream(000);

	SavedTicks = 0;

	SavedTicksSinceSave = 0;

	SavedTicksAtSave = 0;

	SavedStartSeed = 0;

	SavedCurrentStateSeed = 0;

	PlayerCount = 1;

	UnitDetails = TArray<FCUnitSpawnDetails>();

	ControllingPlayers = TArray<int>();
	
	UnlockedAchievements = FGameplayTagContainer();
}
