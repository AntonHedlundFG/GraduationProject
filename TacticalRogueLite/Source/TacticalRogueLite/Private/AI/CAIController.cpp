#include "AI/CAIController.h"
#include "CGameMode.h"
#include "Utility/Logging/CLogManager.h"
#include "Grid/CGridTile.h"
#include "Items/CInventoryComponent.h"
//#include "Items/CItem.h"
#include "Items/ItemSlots.h"

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
/*
float ACAIController::ScoreAction(UCItem* Item, ACGrid* inGrid)
{
	// Score the action based on the item's effects
	// Take considerations into effect
	// Average considerations to get a final score
	return 0;
}
*/
void ACAIController::DecideBestActions()
{
	// Get all tiles reachable by move item

	/* TODO: Change to use itemdata / actioncomponent
	UCItemData* MoveItem = Unit->GetItemInSlot(SharedGameplayTags::ItemSlot_Boots);
	TArray<ACGridTile*> ReachableTiles;
	if(MoveItem == nullptr)
	{
		ReachableTiles.Add(Unit->GetTile());
	}
	else
	{
		ReachableTiles = MoveItem->GetValidTargetTiles(Unit);
	}
	TArray<UCItem*> InventoryItems = Unit->Inventory->GetEquippedItems();
	InventoryItems.RemoveSingle(MoveItem);
	
	TMap<float, TPair<UCItem*, ACGridTile*>> BestActions;
	for (ACGridTile* ReachableTile : ReachableTiles)
	{
		for (UCItem* InventoryItem : InventoryItems)
		{
			float Score = ScoreAction(InventoryItem, ReachableTile->GetParentGrid());
			// Compare with scores in map
			// If more than 5 best actions, replace the worst one with this one
			if(BestActionsMap.Num() < 5)
			{
				BestActions.Add(Score, TPairInitializer(InventoryItem, ReachableTile));
				BestActions.KeySort([](float A, float B)
				{
					return A > B; // Sort in descending order	
				});
			}
			else
			{
				// Get last element
				const float WorstScore = BestActions.end().Key();
				if(Score > WorstScore)
				{
					BestActions.Remove(WorstScore);
					BestActions.Add(Score, TPairInitializer(InventoryItem, ReachableTile));
					
					BestActions.KeySort([](float A, float B)
					{
						return A > B; // Sort in descending order	
					});
				}
			}
		}
	} 


	// If there are multiple best actions, choose one randomly
	const size_t BestActionIndex = FMath::RandRange(0, BestActions.Num() - 1);
	BestActionsMap.Add(MoveItem, BestActions[BestActionIndex].Value); // Add boots first for move
	BestActionsMap.Add(BestActions[BestActionIndex].Key, BestActions[BestActionIndex].Value);

	*/

	// Loop over all possible actions on that tile and score it accordingly
	// TODO: Take negative effects into account
	// If it's possible to move again, evaluate again from the new tile.
	// Store the best action in BestActionsMap
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
