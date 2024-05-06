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


// Sets default values for this component's properties
UCCheatingComponent::UCCheatingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCCheatingComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	GameState = GameMode->GetGameState<ACGameState>();

	SetManyItemCharges(bStartWithManyItemCharges);
}


// Called every frame
void UCCheatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCCheatingComponent::SetManyItemCharges(bool bEnabled)
{
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
}

void UCCheatingComponent::GainBigHealth()
{
	FAttributeModification MaxMod;
	MaxMod.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attribute.HealthMax"));
	MaxMod.Magnitude = 10;
	MaxMod.ModifierOperation = EAttributeModifierOperation::AddBase;

	for (auto* Unit : GameMode->GetHeroUnits())
	{
		Unit->GetActionComp()->ApplyAttributeChange(MaxMod, 0);
	}
}

void UCCheatingComponent::WinThisTurn()
{
	auto* VictoryCondition = GameMode->GetVictoryCondition();
	if (VictoryCondition)
		VictoryCondition->bWonByCheating = true;
}

void UCCheatingComponent::LoseThisTurn()
{
	auto* VictoryCondition = GameMode->GetVictoryCondition();
	if (VictoryCondition)
		VictoryCondition->bLoseByCheating = true;
}

