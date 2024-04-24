class USAction_AoEDamage : UCAction
{
    UPROPERTY()
    int Damage = 1;
    UPROPERTY()
    int Range = 1;
    UPROPERTY()
    bool bCanDamageSelf = false;

    UPROPERTY()
	ACUnit AttackingUnit;

	UPROPERTY()
	TArray<ACUnit> TargetsArray;
    TArray<int> HealthArray;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        AttackingUnit = Cast<ACUnit>(Instigator);
        if(!IsValid(AttackingUnit))
            return;

        TSet<ACGridTile> TilesInRange = GetActionInfluencedTiles(AttackingUnit.GetTile());

        for (ACGridTile Tile : TilesInRange)
        {
            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit TargetUnit = Cast<ACUnit>(Content);

            if (IsValid(TargetUnit) && IsValid(TargetUnit.AttributeComp))
            {
                if(TargetUnit == AttackingUnit && !bCanDamageSelf)
                    continue;

                TargetsArray.Add(TargetUnit);
                HealthArray.Add(TargetUnit.AttributeComp.GetHealth());
            }
        }

        if(TargetsArray.Num() != HealthArray.Num())
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Error, "Error in aquiring targets for AoE action");
            return;
        }
        
        for (int i = 0; i < TargetsArray.Num(); i++)
        {
            //CGameplay::ApplyDamage(AttackingUnit, TargetsArray[i], Damage); TODO: Newattribute

             
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} damaged {TargetsArray[i].GetUnitName()} for <Red> {Damage} </> damage.");
        }
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        for (int i = TargetsArray.Num() - 1; i >= 0; i--)
        {
            TargetsArray[i].GetAttributeComp().SetHealth(HealthArray[i]);
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} undid <Red> {Damage} </> damage on {TargetsArray[i].GetUnitName()}.");
        }
    }

    UFUNCTION(BlueprintOverride)
    TSet<ACGridTile> GetActionInfluencedTiles(ACGridTile fromTile)
    {
        return CGridUtils::FloodFill( fromTile, Range, ActionTags, FGameplayTagContainer());
    }
}