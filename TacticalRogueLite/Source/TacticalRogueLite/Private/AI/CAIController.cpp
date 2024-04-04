#include "AI/CAIController.h"
#include "CGameMode.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Grid/CGridTile.h"
#include "ItemData/CItemData.h"
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

	DecideBestActions();
	ExecuteActions();
	GameMode->TryEndTurn(this);
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

float ACAIController::ScoreAction(FAbility& Ability, ACGrid* inGrid)
{
	// Score the action based on the item's effects
	// Take considerations into effect
	// Average considerations to get a final score
	return 0;
}

void ACAIController::DecideBestActions()
{
	BestActionsMap.Empty();
	if(Unit == nullptr)
	{
		LOG_ERROR("Unit is nullptr for %s", *GetName());
		return;
	}

	// Get all abilities
	TArray<FAbility> Abilities = Unit->GetEquippedAbilities();
	FGameplayTagContainer MoveAbilitiesTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(SharedGameplayTags::Movement);
	TArray<FAbility> MoveAbilities;
	for (FAbility& Ability : Abilities)
	{
		if(Ability.InventorySlotTag.MatchesAny(MoveAbilitiesTagContainer))
		{
			MoveAbilities.Add(Ability);
			break;
		}
	}
	for (int i = 0; i < MoveAbilities.Num(); ++i)
	{
		FAbility Ability = MoveAbilities[i];
		// Get all valid target tiles
		LOG_INFO("Getting valid target tiles for %s", *Ability.InventorySlotTag.ToString());
	}
}

void ACAIController::ExecuteActions()
{
	/*
	for (auto Action : BestActionsMap)
	{
		const EItemSlots ItemSlot = Action.Key->ItemSlot;
		ACGridTile* Tile = Action.Value;
		// Execute the action
		if (GameMode)
		{
			if(!GameMode->TryAbilityUse(this, Unit, ItemSlot, Tile))
			{
				LOG_ERROR("Ability use failed for %s", *GetName());
				return;
			}
			LOG_INFO("%s used by %s on %s", *ToString(ItemSlot), *GetName(), *Tile->GetName());
		}
	}
	*/
}
