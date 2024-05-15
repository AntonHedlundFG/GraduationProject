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
#include "Items/CInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/Logging/CLogManager.h"

bool ACPlayerController::TryLineCastForGridTile(FVector inStart, FVector Direction, float Distance,
                                                ACGridTile*& outTile)
{
	FHitResult Hit;
	if(!UKismetSystemLibrary::LineTraceSingle(this, inStart,inStart + Direction * Distance,UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,TArray<AActor*>(),EDrawDebugTrace::None,Hit,true))
	{ return false; } // If the line trace fails, return false

	AActor* HitActor = Hit.GetActor();
	if(!HitActor) { return false; } // If the line trace hits no Actor , return false
	
	if(ACGridTile* Tile = Cast<ACGridTile>(HitActor))
	{
		outTile = Tile;
		return true; // If the line trace hits a GridTile, return true
	}

	if(ACUnit* Unit = Cast<ACUnit>(HitActor))
	{
		if(ACGridTile* UnitTile = Unit->GetTile())
		{
			outTile = UnitTile;
			return true; // If the line trace hits a Unit with a tile, return true
		}
	}
	
	return false;
}

bool ACPlayerController::IsWithEditor() const
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

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

	OnUndidAbility.Broadcast();

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
	if (!IsValid(GetGameState()->GetCurrentUnit()))
	{
		LOG_WARNING("No valid unit at front of turn order, cancelling ability use.");
		return;
	}
	if (inTag == FGameplayTag::EmptyTag)
	{
		LOG_WARNING("Invalid item slot, cancelling ability use.");
		return;
	}
	
	UnitCurrentlyUsingAbility = GetGameState()->GetCurrentUnit();

	AOnlinePlayerState* PS = GetPlayerState<AOnlinePlayerState>();
	if (!PS || PS->PlayerIndex != UnitCurrentlyUsingAbility->ControllingPlayerIndex)
	{
		LOG_WARNING("Current unit not under your control");
		return;
	}

	UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp();

	if (ActionComp->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Unit.Killed")))
	{
		LOG_WARNING("Unit is dead, cancelling ability use.");
		return;
	}

	FAbility Ability;
	if (!ActionComp->TryGetAbility(inTag, Ability))
	{
		LOG_WARNING("No item in slot, cancelling ability use.");
		return;
	}

	UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetAttributes(UnitCurrentlyUsingAbility);
	if (!AttributeComponent)
	{
		LOG_WARNING("No attributes component on unit, cancelling ability use.");
		return;
	}
	if (!AttributeComponent->HasRemainingCharges(inTag))
	{
		LOG_WARNING("No charges remaining for item, cancelling ability use.");
		return;
	}

	TagCurrentlyUsed = inTag;

	OnAbilityInitiated.Broadcast();

	HighlightedTiles = ActionComp->GetValidTargetTiles(inTag);

	//If we only have 1 valid target, we don't need to wait for the player to choose targets.
	if (HighlightedTiles.Num() == 1)
	{
		FinalizeAbilityUse(HighlightedTiles[0]);
		return;
	}

	TileCurrentlyTargeted = UnitCurrentlyUsingAbility->GetTile();
	ActionComp->ToggleHighlightOnValidTargetTiles(inTag, TileCurrentlyTargeted, true);

}

void ACPlayerController::FinalizeAbilityUse(ACGridTile* inTargetTile)
{
	// If we have a unit currently using an ability, and a tile currently targeted, we should remove the highlights
	if(UnitCurrentlyUsingAbility && TileCurrentlyTargeted)
	{
		if (UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp())
		{
			ActionComp->ToggleHighlightOnValidTargetTiles(TagCurrentlyUsed, TileCurrentlyTargeted, false);
		}
	}
	
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
	if (GetGameState()->GetCurrentUnit() != UnitCurrentlyUsingAbility)
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

	if (ActionComp->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Unit.Killed")))
	{
		LOG_WARNING("Unit is dead, cancelling ability use.");
		CancelAbilityUse();
		return;
	}

	if (!ActionComp->IsValidTargetTile(TagCurrentlyUsed, inTargetTile))
	{
		LOG_WARNING("Item target not valid, cancelling ability use.");
		CancelAbilityUse();
		return;
	}

	FAbility OutAbility;
	ActionComp->TryGetAbility(TagCurrentlyUsed, OutAbility);
	if (ActionComp->ActiveGameplayTags.HasAny(OutAbility.BlockingTags))
	{
		LOG_WARNING("Units ActionComponent contains blocking tags from Ability");
		return;
	}
	
	OnAbilityUsed.Broadcast();
	
	Server_UseObject(UnitCurrentlyUsingAbility, TagCurrentlyUsed, inTargetTile);
	CancelAbilityUse();
}

void ACPlayerController::CancelAbilityUse()
{
	if(UnitCurrentlyUsingAbility && TileCurrentlyTargeted)
	{
		if (UCActionComponent* ActionComp = UnitCurrentlyUsingAbility->GetActionComp())
		{
			ActionComp->ToggleHighlightOnValidTargetTiles(TagCurrentlyUsed, TileCurrentlyTargeted, false);
		}
	}
	
	ItemSlotCurrentlyUsed = EItemSlots::EIS_None;
	TileCurrentlyTargeted = nullptr;
	UnitCurrentlyUsingAbility = nullptr;
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
	OnTurnEnded.Broadcast();
	Server_TryEndTurn();
}

void ACPlayerController::Server_EquipItem_Implementation(ACUnit* inUnit, UCItemData* Item, UCAction_RollItem* Action)
{
	ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	inUnit->GetInventoryComp()->TryEquipItem(Item);
	Action->StopAction(this);
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
