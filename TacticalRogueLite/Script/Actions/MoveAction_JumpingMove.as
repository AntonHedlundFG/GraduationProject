class USMoveAction_Jumping : UCMovementAction
{
    UPROPERTY()
    int RangeTowardsMid = 1;
    UPROPERTY()
    FGameplayTagContainer BlockingTags;

    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACGridTile inTile)
    {
        TArray<ACGridTile> ReturnTiles;
        if(!IsValid(GetOwningComponent()))
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "MoveAction_JumpingMove: Owning component is valid");
            return ReturnTiles;
        }

        FGameplayTag MovementRangeTag = GameplayTags::Attribute_MovementRange;
        int CurrentValue = 0, BaseValue = 0, Additive = 0, Multiplier = 0;
        GetOwningComponent().GetAttribute(MovementRangeTag, CurrentValue, BaseValue, Additive, Multiplier);

        // Get the active tags of the character for MoveRange calculation
        FGameplayTagContainer ActiveTags = UCAttributeComponent::GetAttributes(GetOwningComponent().GetOwner()).ActiveGameplayTags;
        // TODO: Blocking tags
        // Get the tiles that are within the MoveRange and remove the tiles that are within the InnerBlockedRange
        TSet<FVector2D> OuterTiles = CGridUtils::FloodFillWithCoordinates(inTile.GetGridCoords(), BaseValue, ActiveTags);
        TSet<FVector2D> InnerTiles = CGridUtils::FloodFillWithCoordinates(inTile.GetGridCoords(), BaseValue - RangeTowardsMid, ActiveTags);

        TArray<FVector2D> ReturnTileCoords;

        for (FVector2D Tile : OuterTiles)
        {
            if (!InnerTiles.Contains(Tile))
            {
                ReturnTileCoords.Add(Tile);
            }
        }

        ACGrid Grid = inTile.GetParentGrid();

        for (FVector2D TileCoords : ReturnTileCoords)
        {
            ACGridTile Tile = Grid.GetTileFromCoords(TileCoords);
            if(!IsValid(Tile)) continue;

            ACGridContent Content = Tile.GetContent();

            FGameplayTagContainer UnitBlock;
            UnitBlock.AddTag(FGameplayTag::RequestGameplayTag(n"Unit.IsPlayer"));
            UnitBlock.AddTag(FGameplayTag::RequestGameplayTag(n"Unit.IsEnemy"));
            
            if (!IsValid(Content) || !UCAttributeComponent::GetAttributes(Content).ActiveGameplayTags.HasAny(UnitBlock))
            {
                ReturnTiles.Add(Tile);
            }
        }

        return ReturnTiles;
    }
}