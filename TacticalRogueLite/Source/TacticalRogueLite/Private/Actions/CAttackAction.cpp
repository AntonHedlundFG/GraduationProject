// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CAttackAction.h"

#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"
#include "Attributes/CGameplayFunctionLibrary.h"
#include "Grid/CGridTile.h"
#include "Grid/CGridUtilsLibrary.h"
#include "GridContent/CUnit.h"
#include "Kismet/GameplayStatics.h"

void UCAttackAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	// OldHealth = Attributes->GetHealth();
	
	ActualDamage = UCGameplayFunctionLibrary::ApplyDamage(Instigator, TargetTile->GetContent(), DamageAmount, ActionTags); 

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());

	FString AttackerName = Attacker ? Attacker->GetUnitName() : FString("Unknown Unit");
	FString DefenderName = Defender ? Defender->GetUnitName() : FString("Unknown Unit");

	LOG_GAMEPLAY("%s attacked %s for %d damage.", *AttackerName, *DefenderName, DamageAmount);
}

void UCAttackAction::UndoAction_Implementation(AActor* Instigator)
{
	// UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	// Attributes->SetHealth(OldHealth);

	UCGameplayFunctionLibrary::UndoDamage(Instigator, TargetTile->GetContent(), ActualDamage, ActionTags);

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());

	FString AttackerName = Attacker ? Attacker->GetUnitName() : FString("Unknown Unit");
	FString DefenderName = Defender ? Defender->GetUnitName() : FString("Unknown Unit");

	LOG_GAMEPLAY("%s undid their attack on %s for %d damage.", *AttackerName, *DefenderName, DamageAmount);
	
	Super::UndoAction_Implementation(Instigator);
}

TArray<ACGridTile*> UCAttackAction::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
	
	if (!GetOwningComponent())
	{
		LOG_WARNING("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
		return TArray<ACGridTile*>();	
	}
	

	TArray<ACGridTile*> ReturnTiles;
	TArray<ACGridTile*> Neighbours = inTile->GetNeighbours(false);
	for (int i = 0; i < Neighbours.Num(); ++i)
	{
		ACGridTile* Tile = Neighbours[i];
		const ACGridContent* Content = Tile->GetContent();
		if (Content && Content->IsA(ACUnit::StaticClass()))
		{
			ReturnTiles.Add(Tile);
		}
	}

	return ReturnTiles;
	
}

/*void UCAttackAction::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, int NewHealth,
                                     int Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0 && OwningActor != InstigatorActor)
	{
		
		if (Delta == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		Delta = FMath::Abs(Delta);

		// Return damage sender...
		UCGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, Delta);
	}
}*/
