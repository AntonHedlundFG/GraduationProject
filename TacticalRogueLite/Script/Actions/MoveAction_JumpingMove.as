class USMoveAction_Jumping : UCMovementAction
{
    UPROPERTY()
    int OuterMoveRange = 2;
    UPROPERTY()
    int InnerBlockedRange = 1;
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

        // Get the active tags of the character for MoveRange calculation
        FGameplayTagContainer ActiveTags = UCAttributeComponent::GetAttributes(GetOwningComponent().GetOwner()).ActiveGameplayTags;
        // TODO: Blocking tags
        // Get the tiles that are within the MoveRange and remove the tiles that are within the InnerBlockedRange
        TSet<ACGridTile> OuterTiles = CGridUtils::FloodFill(inTile, OuterMoveRange, ActiveTags, BlockingTags);
    TSet<ACGridTile> InnerTiles = CGridUtils::FloodFill(inTile, InnerBlockedRange, ActiveTags, BlockingTags);

        for (ACGridTile Tile : OuterTiles)
        {
            if (!InnerTiles.Contains(Tile))
            {
                ReturnTiles.Add(Tile);
            }
        }

        return ReturnTiles;
    }
}