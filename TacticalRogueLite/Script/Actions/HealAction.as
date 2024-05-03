class USHealAction : UCTargetableAction
{
    private int ActualHeal;

    UPROPERTY(BlueprintReadWrite)
    int Range;
    ACUnit InsitagorUnit;
    int OldHealth;
    UPROPERTY(BlueprintReadWrite)
    int HealAmount;
    UFUNCTION(BlueprintOverride)


    TArray<ACGridTile> GetValidTargetTiles(ACGridTile inTile)
    {
        UCActionComponent ActionComponent = GetOwningComponent();
        if (ActionComponent == nullptr)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
            return TArray<ACGridTile>();	
        }

        TSet<ACGridTile> TilesInRange = CGridUtils::FloodFill(inTile, Range, ActionTags, FGameplayTagContainer());

        TArray<ACGridTile> ReturnTiles;
        for (ACGridTile Tile : TilesInRange)
        {
            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit Unit = Cast<ACUnit>(Content);

            if (IsValid(Unit))
            {           
                 FGameplayTag TeamTag = (ActionComponent.HasTag(GameplayTags::Unit_IsEnemy)) ? 
                                        GameplayTags::Unit_IsEnemy : GameplayTags::Unit_IsPlayer;
                if(IsValid(Unit) && Unit.GetActionComp().HasTag(TeamTag))
                {
                     ReturnTiles.Add(Tile);
                }
            }
        }
        return ReturnTiles;
    }

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {

         InsitagorUnit = Cast<ACUnit>(Instigator);
         ACUnit From = InsitagorUnit;
         ACUnit To = Cast<ACUnit>(TargetTile.GetContent());

        if(From == To)
        {
             UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} Healed self for <Green>{HealAmount}</>.");
        }
        else
        {
             UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} Healed {To.GetUnitName()} for <Green>{HealAmount}</>.");
        }
    }
    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        ACUnit From = InsitagorUnit;
        ACUnit To = Cast<ACUnit>(TargetTile.GetContent());
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay,f"{From.GetUnitName()} Undid <Green>{HealAmount}</> healing on {To.GetUnitName()}.");
    }
}