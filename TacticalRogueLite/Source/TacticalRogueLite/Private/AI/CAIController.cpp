#include "AI/CAIController.h"
#include "CGameMode.h"
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

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this]()
	{
		GameMode->TryEndTurn(this);
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, .5f, false);
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
	float Score;
	// Score the action based on the item's effects
	// Take considerations into effect
	// Average considerations to get a final score

	// Temp/Test
	ACUnit* TargetUnit = GameMode->GetHeroUnits()[0]; 
	ACGridTile* TargetUnitTile = TargetUnit->GetTile();
	StartTile = Unit->GetTile();

	float StartToTargetUnitDistance = FVector::Dist(StartTile->GetActorLocation(), TargetTile->GetActorLocation());
	float AbilityTargetToTargetUnitDistance = FVector::Dist(TargetTile->GetActorLocation(), TargetUnitTile->GetActorLocation());
	
	Score = StartToTargetUnitDistance / AbilityTargetToTargetUnitDistance ;

	
	UKismetSystemLibrary::DrawDebugString(
		GetWorld(),
		TargetTile->GetActorLocation() + FVector::UpVector * 100,
		FString::Printf(TEXT("Score: %.2f"), Score),
		nullptr,
		FLinearColor::Black,
		.5f);
	
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

void ACAIController::EvalAbilitiesFromTile(ACGridTile* CurrentTile, const TArray<FAbility>& Abilities, TArray<FActionPath>& BestPaths, FActionPath& CurrentPath)
{
	const FGameplayTagContainer MoveAbilitiesTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(SharedGameplayTags::Movement);

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

void ACAIController::ExecuteActions(FActionPath& BestActions)
{
	if(GameMode)
	{
		for (TPair<FAbility, ACGridTile*> Path : BestActions.GetPath())
		{
			if(!GameMode->TryAbilityUse(this, Unit, Path.Key.InventorySlotTag, Path.Value))
			{
				LOG_ERROR("Ability use failed for %s", *GetName());
				return;
			}
		}
	}	
}
