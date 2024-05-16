class USAttackAction_Ranged : UCAttackAction
{
    UPROPERTY()
    int AttackRange = 1;

    UPROPERTY()
    FGameplayTagContainer BlockingTags;


    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACGridTile inTile)
    {
        if (GetOwningComponent() == nullptr)
        {
            CLogManager::Log(ELogCategory::LC_Error, "GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
            return TArray<ACGridTile>();	
        }

        FAttribute OutAttribute;
        int Current = 0;
        int Base = 0;
        int Add = 0;
        int Multi = 0;
        GetOwningComponent().GetAttribute(FGameplayTag::RequestGameplayTag(n"Attribute.AttackRange"), Current, Base, Add, Multi);

        TSet<ACGridTile> TilesInRange = CGridUtils::FloodFillWithCoordinatesForTiles( inTile.GetParentGrid(), inTile.GetGridCoords(), Current, AttackDirectionMovementTags, BlockingTags);



        TArray<ACGridTile> ReturnTiles;
        for (ACGridTile Tile : TilesInRange)
        {
            if(Tile == nullptr)
                continue;

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