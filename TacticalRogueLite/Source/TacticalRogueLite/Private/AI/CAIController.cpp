#include "AI/CAIController.h"
#include "CGameMode.h"
#include "AI/CConsideration.h"
#include "Attributes/CAttributeComponent.h"
#include "Grid/CGridTile.h"
#include "ItemData/CItemData.h"
#include "UI/Debug/CAiDebugWindow.h"
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
	LOG_INFO("AI Controller is taking turn with unit: %s.", *Unit->GetUnitName());


	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	const float TimerDelay = FMath::RandRange(0.7f, 1.5f);
	TimerDel.BindLambda([this]()
	{
		// Start Turn
		ExecuteTurn();
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimerDelay, false);

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
		// LOG_INFO("Evaluating consideration %s for %s", *Consideration->GetName(), *Ability.InventorySlotTag.ToString());
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
		const float ModFactor = 1 - (1.0f / NumConsiderations);
		const float MakeUpValue = (1 - OriginalScore) * ModFactor;
		Score += OriginalScore + (MakeUpValue * OriginalScore);
	}
	else
	{
		Score = 1;
		LOG_WARNING("No considerations for %s on Unit: %s -> Returning 1 for consideration score", *Ability.InventorySlotTag.ToString(), *Unit->GetUnitName());
	}
	
	return Score;
}

FActionPath ACAIController::DecideBestActions()
{
	if(!Unit)
	{
		return FActionPath();
	}
	// Init containers
	const TArray<FAbility> Abilities = Unit->GetEquippedAbilities();
	ACGridTile* UnitTile = Unit->GetTile();

	// Recursively score all possible actions
	FActionPath InitialPath;
	EvaluatedPaths = 0;
	EvalAbilitiesFromTile(UnitTile, Abilities, BestPaths, InitialPath);

	if(BestPaths.Num() == 0)
	{
		BestPaths.Add(FActionPath());
	}
	
	return BestPaths[0];
}

void ACAIController::EvalAbilitiesFromTile(ACGridTile* CurrentTile, TArray<FAbility> Abilities, TArray<FActionPath>& inBestPaths, FActionPath& CurrentPath)
{
	EvaluatedPaths++;
	
	// Evaluate all abilities from the current tile
	for (FAbility Ability : Abilities)
	{
		// Skip abilities that have already been used in this path
		if (CurrentPath.HasUsedAbility(Ability))
		{
			continue; 
		}
		
		TArray<ACGridTile*> ReachableTiles = Ability.GetValidTargetTiles(CurrentTile);

		for (int i = 0; i < ReachableTiles.Num(); ++i)
		{
			ACGridTile* Tile = ReachableTiles[i];
			const float Score = ScoreAction(Ability, CurrentTile, Tile);
			
			if(Score == 0) continue; // Skip paths with invalid actions
			
			FActionPath NewPath = CurrentPath;
			NewPath.AddToPath(Ability, Tile, Score);
			
			EvalAbilitiesFromTile(Tile, Abilities, inBestPaths, NewPath);
			
		}
	}
	
	// Try to add the path to the best paths
	TryAddBestPath(CurrentPath, inBestPaths);
	
}

void ACAIController::TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& inBestPaths)
{
	// Keep only top 5 actions
	if (inBestPaths.Num() < 5 || NewPath.GetScore() > inBestPaths.Last().GetScore())
	{
		if(inBestPaths.Num() >= 5)
		{
			FActionPath ActionPath = inBestPaths.Top();
			// LOG_INFO("Removing action with score %f", ActionPath.GetScore());
			inBestPaths.Pop();
		}
		inBestPaths.Add(NewPath);
	}

	// Ensure best actions are sorted by score
	inBestPaths.Sort([](const FActionPath& A, const FActionPath& B)
	{
		return A.GetScore() > B.GetScore();
	});
}

float TimeTotal = 0;

void ACAIController::ExecuteActions(FActionPath BestActions)
{
	if(GameMode)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		const float TimerDelay = FMath::RandRange(.1f, .5f);
		TimeTotal += TimerDelay;
		TArray<TPair<FAbility, ACGridTile*>> Path = BestActions.GetPath();
		if(Path.Num() > 0)
		{
			// Execute the top action and remove it from the path
			const TPair<FAbility, ACGridTile*> Ability = MoveTemp(BestActions.GetPath()[0]);
			BestActions.GetPath().RemoveAt(0);
			
			// Try to use the ability, Error log if it fails
			if(!GameMode->TryAbilityUse(this, Unit, Ability.Key.InventorySlotTag, Ability.Value))
			{
				FString UnitName;
				if(Unit)
				{
					UnitName = Unit->GetUnitName();
				}
				LOG_ERROR("Ability use of %s failed for AI with Unit: %s", *Ability.Key.InventorySlotTag.ToString(), *UnitName);
			}
			
			// Set a timer to execute the next action
			TimerDel.BindLambda([this, BestActions]()
			{
				ExecuteActions(BestActions);
			});
		}
		else
		{
			if(Unit)
			{
				LOG_INFO("Ending Turn for AI Unit: %s", *Unit->GetUnitName());
			}
			// End turn if no actions left
			TimerDel.BindLambda([this]()
			{
				GameMode->TryEndTurn(this);
				TimeTotal = 0;
			});
		}
		// Set the timer
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeTotal, false);
	}	
}

void ACAIController::UpdateContext()
{
	Context.CurrentUnit = Unit;
	Context.PlayerUnits = GameMode->GetHeroUnits();

	// TODO: Remove dead units from the context and add new when spawned based on event instead?
	for (int i = Context.PlayerUnits.Num() - 1; i >= 0; --i)
	{
		const ACUnit* HeroUnit = Context.PlayerUnits[i];
		if(HeroUnit == nullptr || !HeroUnit->GetAttributeComp()->IsAlive())
		{
			Context.PlayerUnits.RemoveAt(i);
		}
	}
	Context.AIUnits = GameMode->GetEnemyUnits();
	for (int i = Context.AIUnits.Num() - 1; i >= 0; --i)
	{
		const ACUnit* AIUnit = Context.AIUnits[i];
		if(AIUnit == nullptr || !AIUnit->GetAttributeComp()->IsAlive())
		{
			Context.AIUnits.RemoveAt(i);
		}
	}
}

void ACAIController::ExecuteTurn()
{
	UpdateContext();
	const auto actions = DecideBestActions();

#if UE_EDITOR
	// Debugging
	FAiDebugInfo Package;
	Package.Instigator = Unit->GetUnitName();
	Package.TotalPathsEvaluated = EvaluatedPaths;
	Package.ActionPaths = BestPaths;
	
	UCAiDebugWindow::GetInstance()->AddPackage(Package);
#endif
	

	
	ExecuteActions(actions);
}
