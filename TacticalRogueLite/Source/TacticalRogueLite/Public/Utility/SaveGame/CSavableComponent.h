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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
};
