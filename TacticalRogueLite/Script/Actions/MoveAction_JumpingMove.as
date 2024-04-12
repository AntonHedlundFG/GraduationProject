class USMoveAction_Jumping : UCMovementAction
{
    UPROPERTY()
    int OuterMoveRange = 2;
    UPROPERTY()
    int InnerBlockedRange = 1;

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
        TSet<ACGridTile> Outer = CGridUtils::FloodFill(inTile, OuterMoveRange, ActiveTags);
        TSet<ACGridTile> Inner = CGridUtils::FloodFill(inTile, InnerBlockedRange, ActiveTags, false);

        for (ACGridTile Tile : Outer)
        {
            if (!Inner.Contains(Tile))
            {
                ReturnTiles.Add(Tile);
            }
        }

        return ReturnTiles;
    }
}