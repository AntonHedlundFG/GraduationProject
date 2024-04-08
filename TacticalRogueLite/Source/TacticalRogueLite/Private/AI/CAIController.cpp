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

	auto actions = DecideBestActions();
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
	for (const UCConsideration* Consideration : Ability.Considerations)
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
	
	UKismetSystemLibrary::DrawDebugString(
		GetWorld(),
		TargetTile->GetActorLocation() + FVector::UpVector * 100,
		FString::Printf(TEXT("Score: %.1f"), Score),
		nullptr,
		FLinearColor::Black,
		4.5f);
	
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
	FActionPath InitialPath;
	EvalAbilitiesFromTile(UnitTile, Abilities, BestPaths, InitialPath);

	return BestPaths[0];
}

void ACAIController::EvalAbilitiesFromTile(ACGridTile* CurrentTile, const TArray<FAbility>& Abilities, TArray<FActionPath>& BestPaths, const FActionPath& CurrentPath)
{
	const FGameplayTagContainer MoveAbilitiesTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(Tag_Movement);

	for (FAbility Ability : Abilities)
	{
		if (CurrentPath.HasUsedAbility(Ability))
		{
			continue; // Skip abilities that have already been used in this path
		}
		
		TArray<ACGridTile*> ReachableTiles = Ability.GetValidTargetTiles(CurrentTile);
        
		for (ACGridTile* Tile : ReachableTiles)
		{
			const float Score = ScoreAction(Ability, CurrentTile, Tile);
			FActionPath NewPath = CurrentPath;
			NewPath.AddToPath(Ability, Tile, Score);

			// If it's a movement ability, recursively evaluate the next tile
			if(Ability.ActionTags.HasAny(MoveAbilitiesTagContainer))
			{
				EvalAbilitiesFromTile(Tile, Abilities, BestPaths, NewPath);
			}
			else
			{
				// If it's not a movement ability, path ends here
				// Add the path to the best paths
				TryAddBestPath(NewPath, BestPaths);
			}
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
		float TimerDelay = FMath::RandRange(0.5f, 1.5f);
		TArray<TPair<FAbility, ACGridTile*>> Path = BestActions.GetPath();
		if(Path.Num() > 0)
		{
			const TPair<FAbility, ACGridTile*> Ability = Path.Top();
			BestActions.GetPath().Pop();
			if(!GameMode->TryAbilityUse(this, Unit, Ability.Key.InventorySlotTag, Ability.Value))
			{
				LOG_ERROR("Ability use failed for %s", *GetName());
			}

			TimerDel.BindLambda([this, BestActions]()
			{
				ExecuteActions(BestActions);
			});
		}
		else
		{
			TimerDel.BindLambda([this]()
			{
				GameMode->TryEndTurn(this);
			});
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimerDelay, false);
	}	
}
