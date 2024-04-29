#pragma once

#include "CoreMinimal.h"
#include "Actions/CTargetableAction.h"
#include "CHighlightTileAction.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCHighlightTileAction : public UCTargetableAction
{
	GENERATED_BODY()
public:
	virtual void StartAction(AActor* Instigator) override;
	
	void SetAbilityToHighlight(const FAbility& inAbility) { AbilityToHighlight = inAbility; }
	void SetDuration(float inDuration) { HighlightDuration = inDuration; }
	void SetFromTile(ACGridTile* inTile) { FromTile = inTile; }

	float GetDuration() const { return HighlightDuration; }
	TArray<ACGridTile*>& GetAffectedTiles() { return AffectedTiles; }
	TArray<ETileHighlightModes>& GetHighlightModes() { return HighlightModes; }
	
protected:
	UPROPERTY(Replicated)
	FAbility AbilityToHighlight;

	UPROPERTY(Replicated)
	ACGridTile* FromTile;
	
	UPROPERTY(Replicated)
	float HighlightDuration = 1.0f;

private:
	UPROPERTY(Replicated)
	TArray<ACGridTile*> AffectedTiles;

	UPROPERTY(Replicated)
	TArray<ETileHighlightModes> HighlightModes;
	
};
