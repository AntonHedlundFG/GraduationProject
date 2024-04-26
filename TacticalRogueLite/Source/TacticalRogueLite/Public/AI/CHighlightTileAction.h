#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CHighlightTileAction.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCHighlightTileAction : public UCTargetableAction
{
	GENERATED_BODY()
public:
	virtual TArray<ACGridTile*> GetValidTargetTiles_Implementation(ACGridTile* inTile) override;
	void SetAbilityToHighlight(const FAbility& inAbility) { HighlightedAbility = inAbility; }
	
protected:
	UPROPERTY()
	FAbility HighlightedAbility;
	UPROPERTY()
	float HighlightDuration = 1.0f;
	
};
