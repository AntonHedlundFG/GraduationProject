#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GridContent/CUnit.h"
#include "CAIController.generated.h"

class ACGameMode;
class ACGrid;

UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnTurnChanged();
	virtual void BeginPlay() override;

	//float ScoreAction(UCItem* Item, ACGrid* Grid);
	void DecideBestActions();
	void ExecuteActions();

private:
	UPROPERTY()
	ACUnit* Unit;
	UPROPERTY()
	ACGrid* Grid;
	//UPROPERTY()
	//TMap<TObjectPtr<UCItem>, TObjectPtr<ACGridTile>> BestActionsMap;
	UPROPERTY()
	ACGameMode* GameMode;
};
