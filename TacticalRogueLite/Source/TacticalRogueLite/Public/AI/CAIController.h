#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GridContent/CUnit.h"
#include "Actions/CAction.h"
#include "CAIController.generated.h"

class ACGrid;
class ACGameMode;

UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnTurnChanged();
	virtual void BeginPlay() override;

	float ScoreAction(FAbility& Ability, ACGrid* inGrid);
	void DecideBestActions();
	void ExecuteActions();

private:
	UPROPERTY()
	ACUnit* Unit;
	UPROPERTY()
	ACGrid* Grid;
	UPROPERTY()
	TMap<FAbility, ACGridTile*> BestActionsMap;
	UPROPERTY()
	ACGameMode* GameMode;
};
