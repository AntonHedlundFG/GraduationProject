#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
	virtual void OnSave() = 0;
	virtual void OnLoad() = 0;
	
	virtual void RegisterToSaveManager();
	virtual void UnregisterFromSaveManager();
};