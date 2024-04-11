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

	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	OldHealth = Attributes->GetHealth();
	
	UCGameplayFunctionLibrary::ApplyDamage(Instigator, TargetTile->GetContent(), DamageAmount); 

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());
	UCLogManager::Log(
		ELogCategory::LC_Gameplay,
		(Attacker ? Attacker->GetUnitName() : FString("Unknown Unit"))
			.Append(" attacked ")
			.Append(Attacker ? Defender->GetUnitName() : FString("Unknown Unit"))
			.Append(" for ")
			.Append(FString::FromInt(DamageAmount))
			.Append(" damage.")
	);
}

void UCAttackAction::UndoAction_Implementation(AActor* Instigator)
{
	UCAttributeComponent* Attributes = UCAttributeComponent::GetAttributes(TargetTile->GetContent());
	Attributes->SetHealth(OldHealth);

	ACUnit* Attacker = Cast<ACUnit>(Instigator);
	ACUnit* Defender = Cast<ACUnit>(TargetTile->GetContent());
	UCLogManager::Log(
		ELogCategory::LC_Gameplay,
		(Attacker ? Attacker->GetUnitName() : FString("Unknown Unit"))
			.Append(" undid their attack on  ")
			.Append(Defender ? Defender->GetUnitName() : FString("Unknown Unit"))
			.Append(" for ")
			.Append(FString::FromInt(DamageAmount))
			.Append(" damage.")
	);
	
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
	for (ACGridTile* Tile : inTile->GetNeighbours(false))
	{
		if (Tile->GetContent() != nullptr && Tile->GetContent()->IsA(ACUnit::StaticClass()))
		{
			ReturnTiles.Add(Tile);
		}
	}

	return ReturnTiles;
	
}

void UCAttackAction::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, int NewHealth,
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
}
