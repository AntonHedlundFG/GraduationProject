
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CPickUp.generated.h"


UCLASS()
class TACTICALROGUELITE_API ACPickUp : public ACGridContent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool IsPickedUp() const { return bIsPickedUp; }

protected:
	UPROPERTY()
	bool bIsPickedUp = false;
	
	virtual void HandleOnTileEnter(ACGridContent* inEnterContent) override;
	virtual void HandleOnTileExit(ACGridContent* inExitContent) override;
};
