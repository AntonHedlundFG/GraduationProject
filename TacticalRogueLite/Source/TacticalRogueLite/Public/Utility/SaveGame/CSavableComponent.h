#pragma once

#include "CoreMinimal.h"
#include "CSavable.h"
#include "CSaveGameManager.h"
#include "Components/ActorComponent.h"
#include "CSavableComponent.generated.h"

UCLASS(Abstract)
class TACTICALROGUELITE_API UCSavableComponent : public UActorComponent, public ICSavable
{
	GENERATED_BODY()
	
public:
	// For Implentation in AS and BP
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnSave();
	virtual void OnSave()
	{
		return ReceiveOnSave();
	}

	// For Implentation in AS and BP
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnLoad();
	virtual void OnLoad()
	{
		return ReceiveOnLoad();
	}

<<<<<<< HEAD
	// Register to Save Manager
	UFUNCTION(BlueprintCallable)
	virtual void RegisterToSaveManager() override
	{
		UCSaveGameManager::Get()->RegisterSavable(this);
	}

	// Unregister from Save Manager
	UFUNCTION(BlueprintCallable)
	virtual void UnregisterFromSaveManager() override
	{
		UCSaveGameManager::Get()->UnRegisterSavable(this);
	}

=======
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
};
