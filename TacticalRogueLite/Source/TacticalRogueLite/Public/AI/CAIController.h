#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIContext.h"
#include "GridContent/CUnit.h"
#include "Actions/CAction.h"
#include "CAIController.generated.h"

class ACGrid;
class ACGameMode;

#pragma region FActionPath
USTRUCT()
struct FActionPath
{
	GENERATED_BODY()
private:
	TArray<TPair<FAbility, ACGridTile*>> Path;
	TArray<FAbility> UsedAbilities;
	float TotalScore = 0;
	
public:
	TArray<TPair<FAbility, ACGridTile*>>& GetPath() { return Path; }
	
	void AddToPath(FAbility& inPath, ACGridTile* inTile, float inScore)
	{
		Path.Add(TPair<FAbility, ACGridTile*>(inPath, inTile));
		UsedAbilities.Add(inPath);
		TotalScore += inScore;
	}

	bool HasUsedAbility(const FAbility& Ability) const
	{
		return UsedAbilities.Contains(Ability);
	}

	float GetScore() const
	{
		if(Path.Num() == 0)	{ return 0; } // No score if no path
		return TotalScore; /// Path.Num(); // TODO: This is a very simple scoring system, we should look into improving this
	}
};
#pragma endregion

UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnTurnChanged();
	virtual void BeginPlay() override;

	float ScoreAction(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile);
	FActionPath DecideBestActions();
	void EvalAbilitiesFromTile(ACGridTile* CurrentTile, TArray<FAbility> Abilities, TArray<FActionPath>& BestPaths, FActionPath& CurrentPath);
	void TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& inBestPaths);
	void ExecuteActions(FActionPath BestActions);
	void UpdateContext();
	void ExecuteTurn();

private:
	UPROPERTY()
	ACUnit* Unit;
	UPROPERTY()
	ACGrid* Grid;
	UPROPERTY()
	TArray<FActionPath> BestPaths;
	UPROPERTY()
	int32 EvaluatedPaths;
	UPROPERTY()
	ACGameMode* GameMode;
	UPROPERTY()
	FCAIContext Context;
};
