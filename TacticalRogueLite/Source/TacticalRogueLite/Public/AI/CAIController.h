#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIContext.h"
#include "CHighlightTileAction.h"
#include "GridContent/CUnit.h"
#include "Actions/CAction.h"
#include "CAIController.generated.h"

class ACGameState;
class ACGrid;
class ACGameMode;

#pragma region FActionPath
USTRUCT()
struct FActionPath
{
	GENERATED_BODY()
private:
	TArray<TPair<FAbility, TObjectPtr<ACGridTile>>> Path;
	TArray<FAbility> UsedAbilities;
	float TotalScore = 0;
	
public:
	TArray<TPair<FAbility, TObjectPtr<ACGridTile>>>& GetPath() { return Path; }
	
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

	bool IsValid()
	{
		if(GetScore() == 0 || Path.Num() == 0)
		{
			return false;
		}

		for (auto Pair : Path)
		{
			if(!Pair.Value)
			{
				return false;
			}
		}

		return true;
	}
};
#pragma endregion

UCLASS()
class TACTICALROGUELITE_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// Check if it is the AI's turn
	// If it is, Execute its turn
	UFUNCTION()
	void OnTurnChanged();

	// Score passed ability based on its considerations at the target tile
	float ScoreAction(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile);
	
	// Evaluate scored actions and decide which ones to use
	FActionPath DecideBestActions();

	// Loop through all abilities
	// Recursively evaluate all possible paths
	void EvalAbilitiesFromTile(ACGridTile* CurrentTile, TArray<FAbility> Abilities, TArray<FActionPath>& BestPaths, FActionPath& CurrentPath);

	// Try to add the path to the best paths
	bool TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& inBestPaths);

	// Execute the best path of abilities
	void ExecuteActions(FActionPath& BestPath);

	// Update the AI context
	void UpdateContext();

	// Execute the AI's turn
	void ExecuteTurn();

	// Register Highlighting action
	UCHighlightTileAction* ConstructHighlightAction(FAbility& Ability, ACGridTile* FromTile, ACGridTile* TargetTile);

	// End the AI's turn
	UFUNCTION()
	void EndTurn();

private:
	UPROPERTY()
	TObjectPtr<ACUnit> Unit;
	UPROPERTY()
	TObjectPtr<ACGrid> Grid;
	UPROPERTY()
	TArray<FActionPath> BestPaths;
	UPROPERTY()
	int32 EvaluatedPaths;
	UPROPERTY()
	TObjectPtr<ACGameMode> GameMode;
	UPROPERTY()
	TObjectPtr<ACGameState> GameState;
	UPROPERTY()
	FCAIContext Context;
	UPROPERTY()
	bool bTurnStarted = false;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	int32 PathsToKeepCount = 5;

};
