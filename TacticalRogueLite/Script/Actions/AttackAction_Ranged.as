class USAttackAction_Ranged : UCAttackAction
{
    UPROPERTY()
    int AttackRange = 1;


    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACGridTile inTile)
    {
        if (GetOwningComponent() == nullptr)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
            return TArray<ACGridTile>();	
        }

        TSet<ACGridTile> TilesInRange = CGridUtils::FloodFill(inTile, AttackRange, ActionTags, false);



        TArray<ACGridTile> ReturnTiles;
        for (ACGridTile Tile : TilesInRange)
        {
            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit Unit = Cast<ACUnit>(Content);

            if (IsValid(Unit))
            {
                ReturnTiles.Add(Tile);
            }
        }

        ReturnTiles.Remove(inTile);

        return ReturnTiles;
    }
}