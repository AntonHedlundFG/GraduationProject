#include "AI/CAIController.h"
#include "CGameMode.h"
#include "Actions/Visualizer/CActionVisualizerSystem.h"
#include "AI/CConsideration.h"
#include "AI/CHighlightTileAction.h"
#include "Attributes/CAttributeComponent.h"
#include "Grid/CGridTile.h"
#include "ItemData/CItemData.h"
#include "UI/Debug/CAiDebugWindow.h"
#include "Utility/CRandomComponent.h"
#include "Utility/Logging/CLogManager.h"

void ACAIController::OnTurnChanged()
{
	Grid = GameMode->GetGameGrid();
	if(Grid == nullptr)
	{
		LOG_ERROR("Grid is nullptr for %s", *GetName());
		return;
	}
	Unit = GameState->TurnOrder[0];
	if(Unit->ControllingPlayerIndex != 0) // AI Controller index is 0
	{
		Unit = nullptr;
		return;
	}
	LOG_INFO("AI Controller is taking turn with unit: %s.", *Unit->GetUnitName());

	bTurnStarted = true;


	BestPaths.Empty();
	
	FTimerHandle TimerHandle1;
	FTimerDelegate TimerDel1;
	TimerDel1.BindLambda([this]()
	{
		ExecuteTurn();
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, TimerDel1, 0.1f, false);
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

	GameState = GameMode->GetGameState<ACGameState>();

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
	UCRandomComponent* Random = GameState->Random;
	const int32 RandomIndex = Random->GetRandRange(0, BestPaths.Num() - 1, false);
	
	return BestPaths[RandomIndex];
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

void ACAIController::TryAddBestPath(FActionPath& NewPath, TArray<FActionPath>& inBestPaths)
{
	if(NewPath.GetScore() == 0 || NewPath.GetPath().Num() == 0)
	{
		return;
	}
	
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

	// Ensure that the best actions are sorted by score
	inBestPaths.Sort([](const FActionPath& A, const FActionPath& B)
	{
		return A.GetScore() > B.GetScore();
	});
}
void ACAIController::ExecuteActions(FActionPath& BestPath)
{
	if(GameMode)
	{
		for (auto Pair : BestPath.GetPath()) 
		{
			// Execute the top action and remove it from the path
			FAbility& Ability = Pair.Key;
			ACGridTile* Tile = Pair.Value;

			// UCHighlightTileAction* HighlightAction = NewObject<UCHighlightTileAction>(this);
			// HighlightAction->SetAbilityToHighlight(Ability);
			// FAbility HighlightAbility;
			// HighlightAbility.InstantiatedActions.Add(HighlightAction);
			//
			// GameMode->TryAbilityUse(this, Unit, HighlightAbility.InventorySlotTag, Tile);

			
			if(!GameMode->TryAbilityUse(this, Unit, Ability.InventorySlotTag, Tile))
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
	FActionPath Actions = DecideBestActions();

#if UE_EDITOR
	// Debugging
	FAiDebugInfo Package;
	Package.Instigator = Unit->GetUnitName();
	Package.TotalPathsEvaluated = EvaluatedPaths;
	Package.ActionPaths = BestPaths;
	
	UCAiDebugWindow::GetInstance()->AddPackage(Package);
#endif
	
	if(Actions.GetPath().Num() > 0)
	{
		ExecuteActions(Actions);
	}
	else
	{
		EndTurn();
	}
}

void ACAIController::EndTurn()
{
	if(bTurnStarted)
	{
		bTurnStarted = false;
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		const float TimerDelay = FMath::RandRange(.7f, 1.5f);
		TimerDel.BindLambda([this]()
		{
			GameMode->TryEndTurn(this);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimerDelay, false);
	}
}
