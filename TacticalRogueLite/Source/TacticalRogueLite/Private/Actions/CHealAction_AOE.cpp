// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CHealAction_AOE.h"
#include "GridContent\CUnit.h"
#include "Grid\CGridTile.h"
#include "Grid\CGridUtilsLibrary.h"
#include "Utility\Logging\CLogManager.h"
#include "Actions\CActionComponent.h"
#include "Net\UnrealNetwork.h"


void UCHealAction_AOE::StartAction(AActor* Instigator)
{

    AttackingUnit = Cast<ACUnit>(Instigator);
    if (!IsValid(AttackingUnit))
        return;


    TSet<ACGridTile*> TilesInRange = GetActionInfluencedTiles(AttackingUnit->GetTile());
    
    ACUnit* Owner = Cast<ACUnit>(GetOwningComponent()->GetOwner());
    if (!IsValid(Owner))
    {
        LOG_ERROR("GetTargetsInRange found no Owner, cannot reach AttributeComponent");
        return;
    }

    FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Unit.IsPlayer");
    FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag("Unit.IsEnemy");
    FGameplayTag TeamTag = Owner->GetGameplayTags().HasTag(PlayerTag) ? (PlayerTag) : (EnemyTag);

    for (ACGridTile* Tile : TilesInRange)
    {
        if (!IsValid(Tile))
            continue;
        
        ACGridContent* Content = Tile->GetContent();
        if (!IsValid(Content))
            continue;

        ACUnit* TargetUnit = Cast<ACUnit>(Content);

        if (IsValid(TargetUnit) && TargetUnit->GetGameplayTags().HasTag(TeamTag))
        {
            if (TargetUnit == AttackingUnit && !bCanHealSelf)
                continue;
            TargetsArray.Add(TargetUnit);
        }
    }

    if (TargetsArray.Num() < 0)
    {
        LOG_ERROR("No targets in Targetsarray");
        return;
    }

    for (int i = 0; i < TargetsArray.Num(); i++)
    {
        for (FAttributeModification& Mod : ModifiersAppliedToTarget)
        {
            UCActionComponent* TargetActionComp = TargetsArray[i]->GetActionComp();
            if (TargetActionComp == nullptr) continue;

            Mod.InstigatorComp = TargetActionComp;
            int ActualDelta = TargetActionComp->ApplyAttributeChange(Mod, 0);
            ModifiersTargetActualDeltas.Add(ActualDelta);
            LOG_GAMEPLAY("%s healed %s for <Green> %d </> health.", *AttackingUnit->GetUnitName(), *TargetsArray[i]->GetUnitName(), ActualDelta);
        }
    }
}

void UCHealAction_AOE::UndoAction(AActor* Instigator)
{
    for (int i = TargetsArray.Num() - 1; i >= 0; i--)
    {
        for (int j = ModifiersAppliedToTarget.Num() - 1; j >= 0; j--)
        {
            UCActionComponent* TargetActionComp = TargetsArray[i]->GetActionComp();
            if (TargetActionComp == nullptr) continue;

            FAttributeModification Mod = ModifiersAppliedToTarget[j];
            Mod.InstigatorComp = TargetActionComp;
            Mod.bIsUndo = true;
            int ActualDelta = ModifiersTargetActualDeltas.Last();
            ModifiersTargetActualDeltas.RemoveAt(ModifiersTargetActualDeltas.Num() - 1);
            Mod.Magnitude = -ActualDelta;
            TargetActionComp->ApplyAttributeChange(Mod, 0);
            LOG_GAMEPLAY("%s undid healing %s for <Green> %d </> health.", *AttackingUnit->GetUnitName(), *TargetsArray[i]->GetUnitName(), ActualDelta);
        }
    }
    Super::UndoAction(Instigator);
}


TSet<ACGridTile*> UCHealAction_AOE::GetActionInfluencedTiles_Implementation(ACGridTile* fromTile)
{
    return UCGridUtilsLibrary::FloodFillWithCoordinatesForTiles(fromTile->GetParentGrid(), fromTile->GetGridCoords(), Range, ActionTags, FGameplayTagContainer());
}

void UCHealAction_AOE::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCHealAction_AOE, AttackingUnit);
    DOREPLIFETIME(UCHealAction_AOE, TargetsArray);
}