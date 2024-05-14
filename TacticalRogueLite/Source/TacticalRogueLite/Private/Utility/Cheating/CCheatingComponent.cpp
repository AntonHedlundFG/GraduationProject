// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Cheating/CCheatingComponent.h"
#include "CGameMode.h"
#include "CGameState.h"
#include "Attributes/CAttributeComponent.h"
#include "GameplayTagsManager.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Achievements/CVictoryCondition.h"
#include "Actions/CActionComponent.h"
#include "Attributes/Utilities/CAttribute.h"

#if !UE_BUILD_SHIPPING

// Called when the game starts
void UCCheatingComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	GameState = GameMode->GetGameState<ACGameState>();

	SetManyItemCharges(bStartWithManyItemCharges);
}

#endif

void UCCheatingComponent::SetManyItemCharges(bool bEnabled)
{
#if !UE_BUILD_SHIPPING

	if (bHasManyItemCharges == bEnabled) return;

	bHasManyItemCharges = bEnabled;
	const int32 ChargesToAdd = bEnabled ? ManyItemChargesAmount : -ManyItemChargesAmount;
	for (auto* Unit : GameMode->GetHeroUnits())
	{
		UCAttributeComponent* Attributes = Unit->GetAttributeComp();
		FGameplayTagContainer ItemSlotTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_ItemSlot);
		for (FGameplayTag ItemSlot : ItemSlotTags)
		{
			Attributes->AddMaxCharges(ItemSlot, ChargesToAdd);
		}
	}
#endif
}

void UCCheatingComponent::GainBigHealth()
{
#if !UE_BUILD_SHIPPING
	FAttributeModification MaxMod;
	MaxMod.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attribute.HealthMax"));
	MaxMod.Magnitude = 10;
	MaxMod.ModifierOperation = EAttributeModifierOperation::AddBase;

	for (auto* Unit : GameMode->GetHeroUnits())
	{
		Unit->GetActionComp()->ApplyAttributeChange(MaxMod, 0);
	}
#endif
}

void UCCheatingComponent::WinThisTurn()
{
#if !UE_BUILD_SHIPPING
	auto* VictoryCondition = GameMode->GetVictoryCondition();
	if (VictoryCondition)
		VictoryCondition->bWonByCheating = true;

#endif
}

void UCCheatingComponent::LoseThisTurn()
{
#if !UE_BUILD_SHIPPING

	auto* VictoryCondition = GameMode->GetVictoryCondition();
	if (VictoryCondition)
		VictoryCondition->bLoseByCheating = true;

#endif
}
