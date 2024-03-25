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
<<<<<<< HEAD
	virtual void RegisterToSaveManager() = 0;
	virtual void UnregisterFromSaveManager() = 0;
=======
	
	virtual void RegisterToSaveManager();
	virtual void UnregisterFromSaveManager();
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
};