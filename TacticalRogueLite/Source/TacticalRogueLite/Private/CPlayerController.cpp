// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "CGameState.h"
#include "Items\CItem.h"
#include "Grid\CGridTile.h"
#include "CGameMode.h"
#include "Actions/CActionComponent.h"
#include "Grid/Tiles/TileHighlightModes.h"

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
	Server_TryUndo();
}

void ACPlayerController::InitiateAbilityUse(FGameplayTag inTag)
{
	CancelAbilityUse();
	if (!GetGameState())
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameState available, cancelling ability use."));
		return;
	}
	if (GetGameState()->TurnOrder.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid unit at front of turn order, cancelling ability use."));
		return;
	}
	if (inTag == FGameplayTag::EmptyTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item slot, cancelling ability use."));
		return;
	}
	
	UnitCurrentlyUsingAbility = GetGameState()->TurnOrder[0];
	UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp();
	FAbility OutAbility;
	if (!ActionComp->TryGetAbility(inTag, OutAbility))
	{
		UE_LOG(LogTemp, Warning, TEXT("No item in slot, cancelling ability use."));
		return;
	}

	TagCurrentlyUsed = inTag;

	HighlightedTiles = ActionComp->GetValidTargetTiles(inTag);
	for (ACGridTile* Tile : HighlightedTiles)
	{
		Tile->OnHighlightChange.Broadcast(true);
		Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Reachable);
	}
	UE_LOG(LogTemp, Warning, TEXT("%d number of tiles"), HighlightedTiles.Num());
}

void ACPlayerController::FinalizeAbilityUse(ACGridTile* inTargetTile)
{
	if (!inTargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Tile nullptr, cancelling ability use."));
		CancelAbilityUse();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Tile Clicked: %s"), *inTargetTile->GetName());

	if (!UnitCurrentlyUsingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability initiated, cancelling ability use."));
		CancelAbilityUse();
		return;
	}
	if (!GetGameState())
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameState available, cancelling ability use."));
		CancelAbilityUse();
		return;
	}
	if (GetGameState()->TurnOrder.Num() == 0 || GetGameState()->TurnOrder[0] != UnitCurrentlyUsingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid unit at front of turn order, cancelling ability use."));
		CancelAbilityUse();
		return;
	}
	/*
	if (ItemSlotCurrentlyUsed == EItemSlots::EIS_None || ItemSlotCurrentlyUsed == EItemSlots::EIS_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item slot, cancelling ability use."));
		CancelAbilityUse();
		return;
	}

	UCItem* UsedItem = UnitCurrentlyUsingAbility->GetItemInSlot(ItemSlotCurrentlyUsed);
	if (!UsedItem || !UsedItem->IsValidTargetTile(UnitCurrentlyUsingAbility, inTargetTile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item target not valid, cancelling ability use."));
		CancelAbilityUse();
		return;
	}*/
	if (TagCurrentlyUsed == FGameplayTag::EmptyTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item slot, cancelling ability use."));
		CancelAbilityUse();
		return;
	}
	UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp();
	if (!ActionComp->IsValidTargetTile(TagCurrentlyUsed, inTargetTile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item target not valid, cancelling ability use."));
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
		Tile->SetTileHighlightMode(ETileHighlightModes::ETHM_Default);
	}
	HighlightedTiles.Empty();
}

void ACPlayerController::EndTurn()
{
	CancelAbilityUse();
	Server_TryEndTurn();
}

#pragma endregion


#pragma region Server RPCs

void ACPlayerController::Server_UseObject_Implementation(ACUnit* inUnit, FGameplayTag inTag, ACGridTile* inTargetTile)
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling ability use."))
			return;
	}
	if (!GameMode->TryAbilityUse(this, inUnit, inTag, inTargetTile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability use failed in GameMode"));
		return;
	}
}

void ACPlayerController::Server_TryUndo_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling undo."))
			return;
	}
	if (!GameMode->TryUndo(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Undoing failed in GameMode"));
		return;
	}
}

void ACPlayerController::Server_TryEndTurn_Implementation()
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ACGameMode available, cancelling end turn."))
			return;
	}
	if (!GameMode->TryEndTurn(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Endturn failed in GameMode"));
		return;
	}
}

#pragma endregion
