#include "AI/CAIController.h"
#include "CGameMode.h"
#include "Actions/Visualizer/CActionVisualizerSystem.h"
#include "AI/CConsideration.h"
#include "AI/CHighlightTileAction.h"
#include "Attributes/CAttributeComponent.h"
#include "Grid/CGridTile.h"
#include "UI/Debug/CAiDebugWindow.h"
#include "Utility/CRandomComponent.h"
#include "Utility/Logging/CLogManager.h"

void ACAIController::OnTurnChanged()
{
	// Check if it's the AI's turn
	Grid = GameMode->GetGameGrid();
	if(Grid == nullptr)
	{
		LOG_ERROR("Grid is nullptr for %s", *GetName());
		return;
	}
	Unit = GameState->TurnOrder[0];
	if(!Unit || Unit->ControllingPlayerIndex != 0) // AI Controller index is 0
	{
		Unit = nullptr;
		return;
	}
	LOG_INFO("AI Controller is taking turn with unit: %s.", *Unit->GetUnitName());

	// Prepare for AI turn
	bTurnStarted = true;
	BestPaths.Empty();

	// Start AI thinking and execute turn after a random delay
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	float GameSpeedMultiplier = 1 / GameState->GetGameSpeed();
	float ThinkingTimer = FMath::RandRange(.1f, 1.0f) * GameSpeedMultiplier;
	TimerDel.BindLambda([this]()
	{
		ExecuteTurn(); 
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, ThinkingTimer, false);
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// Find References
	GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if(!GameMode)
	{
		LOG_ERROR("GameMode is nullptr for %s", *GetName());
		return;
	}
	GameState = GameMode->GetGameState<ACGameState>();
	if(!GameState)
	{
		LOG_ERROR("GameState is nullptr for %s", *GetName());
		return;
	}
	
	// Subscribe to turn change
	GameMode->GetGameState<ACGameState>()->OnTurnOrderUpdate.AddDynamic(this, &ACAIController::OnTurnChanged);

	// Subscribe to action visualizer system to know when to end turn
	UActorComponent* ActionVisualizerSystem = GameState->GetComponentByClass(UCActionVisualizerSystem::StaticClass());
	if(ActionVisualizerSystem)
	{
		UCActionVisualizerSystem* Visualizer = Cast<UCActionVisualizerSystem>(ActionVisualizerSystem);
		Visualizer->OnVisualizationComplete.AddDynamic(this, &ACAIController::EndTurn);
	}
	else
	{
		LOG_ERROR("Action Visualizer System is nullptr for %s", *GetName());
	}
	
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

	// Get a random number between 0 and the number of best paths
	// TODO: Refine how the AI chooses the best path

	// Remove paths too far from the best path
	float BestScore = BestPaths[0].GetScore();
	float Threshold = 0.1f;
	for (int i = BestPaths.Num() - 1; i >= 0; --i)
	{
		if(BestPaths[i].GetScore() < BestScore - Threshold)
		{
			BestPaths.RemoveAt(i);
		}
	}
	
	UCRandomComponent* Random = GameState->Random;
	const int32 RandomIndex = Random->GetRandRange(0, BestPaths.Num() - 1, false);
	
	return BestPaths[RandomIndex];
}

void ACAIController::EvalAbilitiesFromTile(ACGridTile* CurrentTile, TArray<FAbility> Abilities, TArray<FActionPath>& inBestPaths, FActionPath& CurrentPath)
{
	EvaluatedPaths++; // Increment the number of paths evaluated for debugging
	
	// Evaluate all abilities from the current tile
	for (FAbility Ability : Abilities)
	{
		// Skip abilities that have already been used in this path
		if (CurrentPath.HasUsedAbility(Ability))
		{
			continue; 
		}
		
		TArray<ACGridTile*> ReachableTiles = Ability.GetValidTargetTiles(CurrentTile);

		// Evaluate all reachable tiles
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

bool ACAIController::TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& inBestPaths)
{
	// Skip paths with no score
	if(NewPath.GetScore() == 0 || NewPath.GetPath().Num() == 0)
	{
		return false;
	}
	
	// Keep only top N paths
	if (inBestPaths.Num() < PathsToKeepCount || NewPath.GetScore() > inBestPaths.Last().GetScore())
	{
		if(inBestPaths.Num() >= PathsToKeepCount)
		{
			FActionPath ActionPath = inBestPaths.Top();
			inBestPaths.Pop();
		}
		inBestPaths.Add(NewPath);
	}

	// Ensure that the best actions are sorted by score
	inBestPaths.Sort([](const FActionPath& A, const FActionPath& B)
	{
		return A.GetScore() > B.GetScore();
	});

	return true;
}

void ACAIController::ExecuteActions(FActionPath& BestPath)
{
	if(GameMode)
	{
		for (auto Pair : BestPath.GetPath()) 
		{
			// Execute the top action and remove it from the path
			FAbility& Ability = Pair.Key;
			ACGridTile* TargetTile = Pair.Value;

			UCHighlightTileAction* HighlightAction = ConstructHighlightAction(Ability, Unit->GetTile(), TargetTile);
			// Register a highlight action for the ability
			if(HighlightAction)
			{
				// Register the highlight action to allow the visualizer to visualize it before the action is executed
				GameMode->RegisterAction(HighlightAction);
			}
			else
			{
				LOG_ERROR("Highlight Action is nullptr for %s", *GetName());
			}

			// Try to use the ability
			if(!GameMode->TryAbilityUse(this, Unit, Ability.InventorySlotTag, TargetTile))
			{
				FString UnitName;
				if(Unit)
				{
					UnitName = Unit->GetUnitName();
				}
				LOG_ERROR("Ability use of %s failed for AI with Unit: %s", *Ability.InventorySlotTag.ToString(), *UnitName);
			}
		}
	}	
}

void ACAIController::UpdateContext()
{
	Context.CurrentUnit = Unit;
	Context.PlayerUnits = GameMode->GetHeroUnits();
	Context.AIUnits = GameMode->GetEnemyUnits();
}

void ACAIController::ExecuteTurn()
{
	UpdateContext();
	FActionPath Actions = DecideBestActions();

	// Send Paths and their Score to editor UI for Debugging
#if UE_EDITOR
	FAiDebugInfo Package;
	Package.Instigator = Unit->GetUnitName();
	Package.TotalPathsEvaluated = EvaluatedPaths;
	Package.ActionPaths = BestPaths;
	
	UCAiDebugWindow::GetInstance()->AddPackage(Package);
#endif

	// Execute the actions
	if(Actions.GetPath().Num() > 0)
	{
		ExecuteActions(Actions);
	}
	else
	{
		EndTurn();
	}
}

UCHighlightTileAction* ACAIController::ConstructHighlightAction(FAbility& Ability, ACGridTile* FromTile,
                                                                ACGridTile* TargetTile)
{
	// Check if the ability can be used
	if(!Ability.IsValidTargetTile(FromTile, TargetTile))
	{
		LOG_INFO("Ability %s cannot be used on tile %s", *Ability.InventorySlotTag.ToString(), *TargetTile->GetName());
		return nullptr;
	}
	
	// Create a highlight action for the ability
	UCHighlightTileAction* HighlightAction = NewObject<UCHighlightTileAction>(this);

	if(!HighlightAction)
	{
		LOG_ERROR("Highlight Action is nullptr for %s", *GetName());
		return nullptr;
	}
	
	HighlightAction->SetAbilityToHighlight(Ability);
	HighlightAction->TargetTile = TargetTile;
	HighlightAction->SetFromTile(FromTile);
	const float Duration = FMath::RandRange(.1f, 1.0f); // Let visualizer check game speed
	HighlightAction->SetDuration(Duration);

	return HighlightAction;
}

void ACAIController::EndTurn()
{
	if(bTurnStarted)
	{
		bTurnStarted = false;
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		float GameSpeedMultiplier = 1 / GameState->GetGameSpeed();
		const float TimerDelay = FMath::RandRange(.7f, 1.5f) * GameSpeedMultiplier;
		TimerDel.BindLambda([this]()
		{
			GameMode->TryEndTurn(this);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimerDelay, false);
	}
}
