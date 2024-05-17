// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CAttackAction.h"

#include "Actions/CActionComponent.h"
#include "Attributes/CGameplayFunctionLibrary.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"

void UCAttackAction::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	// UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	// OldHealth = Attributes->GetHealth();

	FAttribute Attribute;
	GetOwningComponent()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.AttackDamage"), Attribute);
	ReturnModifications = UCGameplayFunctionLibrary::ApplyDamage(Instigator, TargetTile->GetContent(), Attribute.BaseValue, ActionTags); 

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());
	
	FString AttackerName = Attacker ? Attacker->GetUnitName() : FString("Unknown Unit");
	FString DefenderName = Defender ? Defender->GetUnitName() : FString("Unknown Unit");

	LOG_GAMEPLAY("%s attacked %s for %d damage.", *AttackerName, *DefenderName, Attribute.BaseValue);
}

void UCAttackAction::UndoAction(AActor* Instigator)
{
	// UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	// Attributes->SetHealth(OldHealth);

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());

	for (FAttributeModification Mod : ReturnModifications.Modifications)
	{
		Defender->GetActionComp()->ApplyAttributeChange(Mod, 0);
		FString AttackerName = Attacker ? Attacker->GetUnitName() : FString("Unknown Unit");
		FString DefenderName = Defender ? Defender->GetUnitName() : FString("Unknown Unit");
		
		LOG_GAMEPLAY("%s undid their attack on %s for %d damage.", *AttackerName, *DefenderName, Mod.Magnitude);
	}
	
	Super::UndoAction(Instigator);
}

TArray<ACGridTile*> UCAttackAction::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
	
	if (!GetOwningComponent())
	{
		LOG_WARNING("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
		return TArray<ACGridTile*>();	
	}
	if (!IsValid(inTile))
		return TArray<ACGridTile*>();

	bool bIncludeDiagonals = AttackDirectionMovementTags.HasTag(FGameplayTag::RequestGameplayTag("Movement.Diagonal"));
	
	TArray<ACGridTile*> ReturnTiles;
	TArray<ACGridTile*> Neighbours = inTile->GetNeighbours(bIncludeDiagonals);
	for (int i = 0; i < Neighbours.Num(); ++i)
	{
		ACGridTile* Tile = Neighbours[i];
		if(!Tile) continue;
		const ACGridContent* Content = Tile->GetContent();
		if (Content && Content->IsA(ACUnit::StaticClass()))
		{
			ReturnTiles.Add(Tile);
		}
	}

	return ReturnTiles;
	
}