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
    TArray<FAttributeModifications> UndoDamageList;

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

            if (IsValid(TargetUnit))
            {
                if(TargetUnit == AttackingUnit && !bCanDamageSelf)
                    continue;

                TargetsArray.Add(TargetUnit);
            }
        }

        if(TargetsArray.Num() < 0)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Error, "No targets in Targetsarray");
            return;
        }
        
        for (int i = 0; i < TargetsArray.Num(); i++)
        {
    
            UndoDamageList.Add(CGameplay::ApplyDamage(AttackingUnit, TargetsArray[i], Damage, ActionTags)); 

             
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} damaged {TargetsArray[i].GetUnitName()} for <Red> {Damage} </> damage.");
        }
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        for (int i = TargetsArray.Num() - 1; i >= 0; i--)
        {
           for (FAttributeModification& Mod : UndoDamageList[i].Modifications)
           {
                TargetsArray[i].ActionComp.ApplyAttributeChange(Mod, 0);
                UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} undid <Red> {Mod.Magnitude} </> damage on {TargetsArray[i].GetUnitName()}.");
           }
        }
    }

    UFUNCTION(BlueprintOverride)
    TSet<ACGridTile> GetActionInfluencedTiles(ACGridTile fromTile)
    {
        return CGridUtils::FloodFill( fromTile, Range, ActionTags, FGameplayTagContainer());
    }
}