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
	void SetAbilityToHighlight(const FAbility& inAbility) { AbilityToHighlight = inAbility; }

	FAbility& GetAbility() { return AbilityToHighlight; }
	void SetDuration(float inDuration) { HighlightDuration = inDuration; }
	
protected:
	UPROPERTY(Replicated)
	FAbility AbilityToHighlight;
	
	UPROPERTY()
	float HighlightDuration = 1.0f;
	
};
