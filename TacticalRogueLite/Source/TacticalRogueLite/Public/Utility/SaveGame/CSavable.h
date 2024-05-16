#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utility/Logging/CLogManager.h"
#include "CSavable.generated.h"

UINTERFACE(MinimalAPI)
class UCSavable : public UInterface
{
	GENERATED_BODY()
};

class ICSavable
{
	GENERATED_BODY()

public:
	virtual void OnSave() { LOG_WARNING("Reached default UCSavable->OnSave() implementation, should not happen."); }
	virtual void OnLoad() { LOG_WARNING("Reached default UCSavable->OnLoad() implementation, should not happen."); }
	
	virtual void RegisterToSaveManager() { LOG_WARNING("Savable object needs to implement RegisterToSaveManager for proper functionality"); }
	virtual void UnregisterFromSaveManager() { LOG_ERROR("Savable object needs to implement UnregisterFromSaveManager for proper functionality"); }
};