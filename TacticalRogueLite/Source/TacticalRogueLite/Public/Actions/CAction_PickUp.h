
#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_PickUp.generated.h"

class ACUnit;
class ACPickUp;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAction_PickUp : public UCAction
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ACPickUp> PickUp;
	UPROPERTY()
	TObjectPtr<ACUnit> PickingUnit;
	
	virtual void StartAction(AActor* Instigator) override;
	virtual void StopAction(AActor* Instigator) override;
	virtual void UndoAction(AActor* Instigator) override;

protected:
	UPROPERTY()
	TObjectPtr<ACGridTile> StartTile;

};
