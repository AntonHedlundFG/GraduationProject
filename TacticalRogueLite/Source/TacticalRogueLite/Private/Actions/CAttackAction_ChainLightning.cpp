// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CAttackAction_ChainLightning.h"
#include "Actions\CActionComponent.h"
#include "Grid\CGridTile.h"
#include "GridContent\CUnit.h"
#include "Utility\Logging\CLogManager.h"
#include "Grid\CGridUtilsLibrary.h"
#include "Grid\CGrid.h"
#include "Attributes\CGameplayFunctionLibrary.h"
#include "Net\UnrealNetwork.h"

TArray<ACGridTile*> UCAttackAction_ChainLightning::GetValidTargetTiles_Implementation(ACGridTile* inTile)
{
    if (!IsValid(GetOwningComponent()))
    {
        LOG_ERROR("GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
        return TArray<ACGridTile*>();
    }

    FAttribute RangeAttribute;
    GetOwningComponent()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.AttackRange"), RangeAttribute);
    
    return GetTargetsInRange(inTile, RangeAttribute.GetValue());
    return TArray<ACGridTile*>();
}

void UCAttackAction_ChainLightning::StartAction(AActor* Instigator)
{
    Super::StartAction(Instigator);

    AttackingUnit = Cast<ACUnit>(Instigator);
    ACUnit* Target = Cast<ACUnit>(TargetTile->GetContent());
    if (!IsValid(Target))
    {
        LOG_WARNING("Target is not a unit for Chain Lightning");
        return;
    }

    DamageTarget(Target);

    for (int i = 0; i < BounceCount; i++)
    {
        if (!IsValid(Target)) break;

        TArray<ACGridTile*> TilesInRange = GetTargetsInRange(Target->GetTile(), BounceRange);
        TArray<ACGridContent*> UnitsInRange;

        for (ACGridTile* Tile : TilesInRange)
        {
            ACGridContent* Content = Tile->GetContent();
            if (!IsValid(Content))
                continue;

            ACUnit* Unit = Cast<ACUnit>(Content);

            if (IsValid(Unit) && !UnitsHit.Contains(Unit))
            {
                UnitsInRange.Add(Unit);
            }
        }

        
        Target = Cast<ACUnit>(UCGridUtilsLibrary::GetClosestGridContent(Target->GetTile(), UnitsInRange, ActionTags, FGameplayTagContainer()));
        if (IsValid(Target))
        {
            DamageTarget(Target);
            continue;
        }
    }
}

void UCAttackAction_ChainLightning::UndoAction(AActor* Instigator)
{
    for (int i = UnitsHit.Num() - 1; i >= 0; i--)
    {
        for (FAttributeModification& Mod : ReturnModifications[i].Modifications)
        {
            UnitsHit[i]->GetActionComp()->ApplyAttributeChange(Mod, 0);
            LOG_GAMEPLAY("%s undid <Red> %d </> damage on %s.", *AttackingUnit->GetUnitName(), Mod.Magnitude, *UnitsHit[i]->GetUnitName());
        }
    }
}

TArray<ACGridTile*> UCAttackAction_ChainLightning::GetTargetsInRange(ACGridTile* inTile, int Range)
{
    TSet<FVector2D> TilesInRange = UCGridUtilsLibrary::FloodFillWithCoordinates(inTile->GetGridCoords(), Range, ActionTags);

    ACUnit* Owner = Cast<ACUnit>(GetOwningComponent()->GetOwner());
    if (!IsValid(Owner))
    {
        LOG_ERROR("GetTargetsInRange found no Owner, cannot reach AttributeComponent");
        return TArray<ACGridTile*>();
    }

    FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Unit.IsPlayer");
    FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag("Unit.IsEnemy");

    // Filter invalid tiles
    FGameplayTag OpponentTag = Owner->GetGameplayTags().HasTag(PlayerTag) ? (EnemyTag) : (PlayerTag);
    TArray<ACGridTile*> ReturnTiles;
    for (FVector2D TileCoords : TilesInRange)
    {
        ACGridTile* Tile = inTile->GetParentGrid()->GetTileFromCoords(TileCoords);
        if (!IsValid(Tile))
            continue;

        ACGridContent* Content = Tile->GetContent();
        if (Content == nullptr)
            continue;

        ACUnit* Unit = Cast<ACUnit>(Content);

        if (IsValid(Unit) && Unit->GetGameplayTags().HasTag(OpponentTag))
        {
            ReturnTiles.Add(Tile);
        }
    }
    ReturnTiles.Remove(inTile);

    return ReturnTiles;
}

void UCAttackAction_ChainLightning::DamageTarget(ACUnit* Target)
{
    FAttribute AttackDamageAttr;
    GetOwningComponent()->GetAttribute(FGameplayTag::RequestGameplayTag("Attribute.AttackDamage"), AttackDamageAttr);
    int Damage = FMath::Max(1, AttackDamageAttr.GetValue() - Damaged);
    Damaged++;
    FAttributeModifications Modifications = UCGameplayFunctionLibrary::ApplyDamage(AttackingUnit, Target, AttackDamageAttr.GetValue(), ActionTags);
    ReturnModifications.Add(Modifications);
    UnitsHit.Add(Target);
    LOG_GAMEPLAY("Chain Lightning hit %s", *Target->GetName());
}

void UCAttackAction_ChainLightning::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCAttackAction_ChainLightning, AttackingUnit);
    DOREPLIFETIME(UCAttackAction_ChainLightning, UnitsHit);
}