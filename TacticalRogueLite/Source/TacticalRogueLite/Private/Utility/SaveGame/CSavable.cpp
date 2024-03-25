#include "Utility/SaveGame/CSavable.h"
#include "Utility/SaveGame/CSaveGameManager.h"


void ICSavable::RegisterToSaveManager()
{
	UCSaveGameManager::Get()->RegisterSavable(this);
}

void ICSavable::UnregisterFromSaveManager()
{
	UCSaveGameManager::Get()->UnRegisterSavable(this);
}

