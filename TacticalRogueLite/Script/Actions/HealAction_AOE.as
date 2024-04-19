class USHealAction_AOE: UCAction
{
    UPROPERTY(BlueprintReadWrite)
    int Range;
    UPROPERTY(BlueprintReadWrite)
    int HealAmount;
    ACUnit InsigatorUnit;
    TArray<ACUnit> HealedUnits;
    TArray<int> OldHealth;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        UCActionComponent ActionComponent = GetOwningComponent();
        if(ActionComponent == nullptr)
        {
            return;
        }
        InsigatorUnit = Cast<ACUnit>(Instigator);
        ACGridTile startTile = InsigatorUnit.GetTile();
        TSet<ACGridTile> TilesInRange = GetActionInfluencedTiles(startTile);
        TArray<ACUnit> ValidUnits;

        for(auto Tile :TilesInRange)
        {
            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit Unit = Cast<ACUnit>(Content);
            if(IsValid(Unit))
            {           
                 FGameplayTag TeamTag = (ActionComponent.ActiveGameplayTags.HasTag(GameplayTags::Unit_IsEnemy)) ? 
                                        GameplayTags::Unit_IsEnemy : GameplayTags::Unit_IsPlayer;
                if(IsValid(Unit) && Unit.GetActionComp().ActiveGameplayTags.HasTag(TeamTag))
                {
                     ValidUnits.Add(Unit);
                }
            }
        }

        ACUnit From = Cast<ACUnit>(Instigator);
        ACUnit To;
        
        for(ACUnit Unit : ValidUnits)
        {
            To = Unit;
            CGameplay::ApplyHealing(Instigator,To,HealAmount);
            HealedUnits.Add(To);
            OldHealth.Add(To.GetAttributeComp().GetHealth());
            if(From == To)
            {
                 UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} Healed self for <Green>{HealAmount}</>.");
            }
            else
            {
                 UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} Healed {To.GetUnitName()} for <Green>{HealAmount}</>.");
            }
        }
    }
    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        ACUnit From = InsigatorUnit;
        ACUnit To;
        for(int i = HealedUnits.Num()-1; i >= 0; i--)
        {
            To = HealedUnits[i];
            HealedUnits[i].GetAttributeComp().SetHealth(OldHealth[i]);
            if(From == To)
            {
                 UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} undid <Green>{HealAmount}</> healing on self.");
            }
            else
            {
                 UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} undid <Green>{HealAmount}</> on {To.GetUnitName()}.");
            }
        }
    }

    UFUNCTION(BlueprintOverride)
    TSet<ACGridTile> GetActionInfluencedTiles(ACGridTile fromTile)
    {
        return CGridUtils::FloodFill( fromTile, Range, ActionTags, FGameplayTagContainer());
    }
}