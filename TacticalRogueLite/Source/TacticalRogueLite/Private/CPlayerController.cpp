// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "CGameState.h"
#include "Grid\CGridTile.h"
#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Grid/Tiles/TileHighlightModes.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"
#include "Attributes/CAttributeComponent.h"
#include "Grid/CTileHighlightComponent.h"

ACGameState* ACPlayerController::GetGameState()
{
	if (!GameStateRef && GetWorld())
		GameStateRef = GetWorld()->GetGameState<ACGameState>();
	return GameStateRef;
}

#pragma region Ability Input

void ACPlayerController::UndoAbility()
{
	CancelAbilityUse();

	if (GetGameState() && GetGameState()->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return;
	}

	Server_TryUndo();
}

void ACPlayerController::InitiateAbilityUse(FGameplayTag inTag)
{
	CancelAbilityUse();
	if (!GetGameState())
	{
		LOG_WARNING("No ACGameState available, cancelling ability use.");
		return;
	}
	if (GetGameState()->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return;
	}
	if (GetGameState()->TurnOrder.Num() == 0)
	{
		LOG_WARNING("No valid unit at front of turn order, cancelling ability use.");
		return;
	}
	if (inTag == FGameplayTag::EmptyTag)
	{
		LOG_WARNING("Invalid item slot, cancelling ability use.");
		return;
	}
	
	UnitCurrentlyUsingAbility = GetGameState()->TurnOrder[0];

	AOnlinePlayerState* PS = GetPlayerState<AOnlinePlayerState>();
	if (!PS || PS->PlayerIndex != UnitCurrentlyUsingAbility->ControllingPlayerIndex)
	{
		LOG_WARNING("Current unit not under your control");
		return;
	}

	UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp();
	FAbility OutAbility;
	if (!ActionComp->TryGetAbility(inTag, OutAbility))
	{
		LOG_WARNING("No item in slot, cancelling ability use.");
		return;
	}

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(UnitCurrentlyUsingAbility);
	if (!Attributes)
	{
		LOG_WARNING("No attributes component on unit, cancelling ability use.");
		return;
	}
	if (!Attributes->HasRemainingCharges(inTag))
	{
		LOG_WARNING("No charges remaining for item, cancelling ability use.");
		return;
	}

	TagCurrentlyUsed = inTag;

	HighlightedTiles = ActionComp->GetValidTargetTiles(inTag);

	//If we only have 1 valid target, we don't need to wait for the player to choose targets.
	if (HighlightedTiles.Num() == 1)
	{
		FinalizeAbilityUse(HighlightedTiles[0]);
		return;
	}

	for (ACGridTile* Tile : HighlightedTiles)
	{
		Tile->OnHighlightChange.Broadcast(true);
		Tile->GetHighlightComponent()->AppendHighlightMode(ETileHighlightModes::ETHM_Reachable);
	}
}

void ACPlayerController::FinalizeAbilityUse(ACGridTile* inTargetTile)
{
	if (!inTargetTile)
	{
		LOG_WARNING("Target Tile nullptr, cancelling ability use.");
		CancelAbilityUse();
		return;
	}

	LOG_WARNING("Tile Clicked: %s", *inTargetTile->GetName());

	if (!UnitCurrentlyUsingAbility)
	{
		LOG_WARNING("No ability initiated, cancelling ability use.");
		CancelAbilityUse();
		return;
	}
	if (!GetGameState())
	{
		LOG_WARNING("No ACGameState available, cancelling ability use.");
		CancelAbilityUse();
		return;
	}
	if (GetGameState()->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return;
	}
	if (GetGameState()->TurnOrder.Num() == 0 || GetGameState()->TurnOrder[0] != UnitCurrentlyUsingAbility)
	{
		LOG_WARNING("No valid unit at front of turn order, cancelling ability use.");
		CancelAbilityUse();
		return;
	}
	if (TagCurrentlyUsed == FGameplayTag::EmptyTag)
	{
		LOG_WARNING("Invalid item slot, cancelling ability use.");
		CancelAbilityUse();
		return;
	}

	UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp();
	if (!ActionComp->IsValidTargetTile(TagCurrentlyUsed, inTargetTile))
	{
		LOG_WARNING("Item target not valid, cancelling ability use.");
		CancelAbilityUse();
		return;
	}
	
	Server_UseObject(UnitCurrentlyUsingAbility, TagCurrentlyUsed, inTargetTile);
	CancelAbilityUse();
}

void ACPlayerController::CancelAbilityUse()
{
	ItemSlotCurrentlyUsed = EItemSlots::EIS_None;
	UnitCurrentlyUsingAbility = nullptr;
	for (ACGridTile* Tile : HighlightedTiles)
	{
		Tile->OnHighlightChange.Broadcast(false);
		Tile->GetHighlightComponent()->RemoveHighlightMode(ETileHighlightModes::ETHM_Reachable);
	}
	HighlightedTiles.Empty();
}

void ACPlayerController::EndTurn()
{
	CancelAbilityUse();
	if (!GetGameState())
	{
		LOG_WARNING("No ACGameState available, cancelling ability use.");
		CancelAbilityUse();
		return;
	}
	if (GetGameState()->IsGameOver())
	{
		LOG_INFO("Game is already over");
		return;
	}
	Server_TryEndTurn();
}

#pragma endregion


#pragma region Server RPCs

void ACPlayerController::Server_UseObject_Implementation(ACUnit* inUnit, FGameplayTag inTag, ACGridTile* inTargetTile)
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		LOG_WARNING("No ACGameMode available, cancelling ability use.");
		return;
	}
	if (!GameMode->TryAbilityUse(this, inUnit, inTag, inTargetTile))
	{
		LOG_WARNING("Ability use failed in GameMode");
		return;
	}
}

void ACPlayerController::Server_TryUndo_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		LOG_WARNING("No ACGameMode available, cancelling undo.");
		return;
	}
	if (!GameMode->TryUndo(this))
	{
		LOG_WARNING("Undoing failed in GameMode");
		return;
	}
}

void ACPlayerController::Server_TryEndTurn_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		LOG_WARNING("No ACGameMode available, cancelling end turn.");
		return;
	}
	if (!GameMode->TryEndTurn(this))
	{
		LOG_WARNING("Endturn failed in GameMode");
		return;
	}
}

#pragma endregion
