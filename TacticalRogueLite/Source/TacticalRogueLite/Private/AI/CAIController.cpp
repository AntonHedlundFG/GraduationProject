#include "AI/CAIController.h"
#include "CGameMode.h"
#include "AI/CConsideration.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Grid/CGridTile.h"
#include "ItemData/CItemData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/Logging/CLogManager.h"

void ACAIController::OnTurnChanged()
{
	Grid = GameMode->GetGameGrid();
	if(Grid == nullptr)
	{
		LOG_ERROR("Grid is nullptr for %s", *GetName());
		return;
	}
	Unit = GameMode->GetGameState<ACGameState>()->TurnOrder[0];
	if(Unit->ControllingPlayerIndex != 0) // AI Controller index is 0
	{
		Unit = nullptr;
		return;
	}
	LOG_INFO("AI Controller %s is taking turn", *GetName());

	UpdateContext();
	const auto actions = DecideBestActions();
	ExecuteActions(actions);

	// Turn End through ExecuteActions
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	// Find Game Mode
	GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if(!GameMode)
	{
		LOG_ERROR("GameMode is nullptr for %s", *GetName());
		return;
	}
	// Subscribe to turn change
	GameMode->GetGameState<ACGameState>()->OnTurnOrderUpdate.AddDynamic(this, &ACAIController::OnTurnChanged);
}

float ACAIController::ScoreAction(FAbility& Ability, ACGridTile* StartTile, ACGridTile* TargetTile)
{
	float Score = 1;
	// Score the action based on the item's effects
	// Take considerations into effect
	for (UCConsideration* Consideration : Ability.Considerations)
	{
		const float ConsiderationScore = Consideration->Evaluate(Ability, StartTile, TargetTile, Context);
		Score *= ConsiderationScore;

		if(Score == 0)
		{
			return 0; // If any consideration returns 0, the action is invalid (Will always return 0 after this point)
		}
	}
	// Average considerations to get a final score
	const int32 NumConsiderations = Ability.Considerations.Num();
	if(NumConsiderations > 0)
	{
		const float OriginalScore = Score;
		const float ModFactor = 1 - (1.0f / Ability.Considerations.Num());
		const float MakeUpValue = (1 - OriginalScore) * ModFactor;
		Score += OriginalScore + (MakeUpValue * OriginalScore);
	}
	else
	{
		Score = 0;
		LOG_WARNING("No considerations for %s on Unit: %s", *Ability.InventorySlotTag.ToString(), *Unit->GetUnitName());
	}
	
	return Score;
}

FActionPath ACAIController::DecideBestActions()
{
	BestActionsMap.Empty();
	if(Unit == nullptr)
	{
		LOG_ERROR("Unit is nullptr for %s", *GetName());
		return FActionPath();
	}

	// Init containers
	const TArray<FAbility> Abilities = Unit->GetEquippedAbilities();
	TArray<FActionPath> BestPaths;
	ACGridTile* UnitTile = Unit->GetTile();

	// Recursively score all possible actions
	const FActionPath InitialPath;
	EvalAbilitiesFromTile(UnitTile, Abilities, BestPaths, InitialPath);

	if(BestPaths.Num() == 0)
	{
		BestPaths.Add(FActionPath());
	}
	
	return BestPaths[0];
}

void ACAIController::EvalAbilitiesFromTile(ACGridTile* CurrentTile, TArray<FAbility> Abilities, TArray<FActionPath>& BestPaths, const FActionPath& CurrentPath)
{
	const FGameplayTagContainer MoveAbilitiesTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Movement);

	// Evaluate all abilities from the current tile
	for (FAbility Ability : Abilities)
	{
		// Skip abilities that have already been used in this path
		if (CurrentPath.HasUsedAbility(Ability))
		{
			continue; 
		}
		
		TArray<ACGridTile*> ReachableTiles = Ability.GetValidTargetTiles(CurrentTile);
        
		for (ACGridTile* Tile : ReachableTiles)
		{
			const float Score = ScoreAction(Ability, CurrentTile, Tile);
			
			if(Score == 0) continue; // Skip paths with invalid actions
			
			FActionPath NewPath = CurrentPath;
			NewPath.AddToPath(Ability, Tile, Score);
			
			// If it's a movement ability, recursively evaluate the next tile
			if(Ability.AbilityTags.HasAny(MoveAbilitiesTagContainer))
			{
				EvalAbilitiesFromTile(Tile, Abilities, BestPaths, NewPath);
			}
			
			// If it's not a movement ability, path ends here
			// Add the path to the best paths
			TryAddBestPath(NewPath, BestPaths);
		}
	}
}

void ACAIController::TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& BestPaths)
{
	// Keep only top 5 actions
	if (BestPaths.Num() < 5 || NewPath.GetScore() > BestPaths.Last().GetScore())
	{
		if(BestPaths.Num() >= 5)
		{
			FActionPath ActionPath = BestPaths.Top();
			LOG_INFO("Removing action with score %f", ActionPath.GetScore());
			BestPaths.Pop();
		}
		BestPaths.Add(NewPath);
	}

	// Ensure best actions are sorted by score
	BestPaths.Sort([](const FActionPath& A, const FActionPath& B)
	{
		return A.GetScore() > B.GetScore();
	});
}

void ACAIController::ExecuteActions(FActionPath BestActions)
{
	if(GameMode)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		const float TimerDelay = FMath::RandRange(.1f, .5f);
		TArray<TPair<FAbility, ACGridTile*>> Path = BestActions.GetPath();
		if(Path.Num() > 0)
		{
			// Execute the top action and remove it from the path
			const TPair<FAbility, ACGridTile*> Ability = Path.Top();
			BestActions.GetPath().Pop();
			// Try to use the ability, Error log if it fails
			if(!GameMode->TryAbilityUse(this, Unit, Ability.Key.InventorySlotTag, Ability.Value))
			{
				LOG_ERROR("Ability use of %s failed for AI", *Ability.Key.InventorySlotTag.ToString());
			}
			// Set a timer to execute the next action
			TimerDel.BindLambda([this, BestActions]()
			{
				ExecuteActions(BestActions);
			});
		}
		else
		{
			// End turn if no actions left
			TimerDel.BindLambda([this]()
			{
				GameMode->TryEndTurn(this);
			});
		}
		// Set the timer
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimerDelay, false);
	}	
}

void ACAIController::UpdateContext()
{
	Context.CurrentUnit = Unit;
	Context.PlayerUnits = GameMode->GetHeroUnits();
	Context.AIUnits = GameMode->GetEnemyUnits();
}
