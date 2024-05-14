class USAction_PullInGridContent : UCAction
{
    UPROPERTY()
    int Range = 2;
    UPROPERTY()
    int MoveTileCount = 1;

    UPROPERTY(Replicated)
    TArray<ACGridContent> ContentInRange;

    UPROPERTY(Replicated)
    TArray<ACGridTile> ContentStartTiles;
    
    UPROPERTY(Replicated)
    ACGridTile TargetTile;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        ACGridContent InstigatorContent = Cast<ACGridContent>(Instigator);
        if(!IsValid(InstigatorContent))
        { 
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "PullInUnit Action found no OwningComponent");
            return;
        }
        TargetTile = InstigatorContent.GetTile();
        
        TSet<ACGridTile> TilesInRange = GetActionInfluencedTiles(TargetTile);

        // Save all units in range and their original tiles for undo
        for(ACGridTile Tile : TilesInRange)
        {
            ACGridContent TileContent = Cast<ACGridContent>(Tile.GetContent());
            if(IsValid(TileContent) && TileContent != InstigatorContent && TileContent.GridContentTags.HasAny(ActionTags))
            {
                ContentInRange.Add(TileContent);
                ContentStartTiles.Add(TileContent.GetTile());
            }
        }

        // Move units towards the instigator
        for( ACGridContent Content : ContentInRange)
        {
                // Pathfind towards StartTile
                auto Path = CGridUtils::BFS_Pathfinding(Content.GetTile(), TargetTile, ActionTags, ActionBlockingTags);
                for (int i = 1; i <= MoveTileCount; i++)
                {
                    if(i >= Path.Num())
                    {
                        break;
                    }

                    ACGridTile NextTile = Path[i];
                    if(NextTile.GetContent() == nullptr)
                    {
                        Content.SetTile(NextTile);
                    }else
                    {
                        break;
                    }
                }
        }

        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{InstigatorContent.GetName()} pulled in units in range {Range} by {MoveTileCount} tiles.");

    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        // Move units back to their original tiles in reverse order to avoid conflicts
        for( int i = ContentInRange.Num() - 1; i >= 0; i--)
        {
            ACGridContent Content = ContentInRange[i];
            if(IsValid(Content) && IsValid(Content.GetTile()))
            {
                Content.SetTile(ContentStartTiles[i]);
            }
        }
        
        //ContentStartTiles.Empty();
        //ContentInRange.Empty();
        
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{Instigator.GetName()} pulled in units in range {Range} by {MoveTileCount} tiles.");
    }

    UFUNCTION(BlueprintOverride)
    TSet<ACGridTile> GetActionInfluencedTiles(ACGridTile fromTile)
    {
        return CGridUtils::FloodFill( fromTile, Range, ActionTags, ActionBlockingTags);
    }
}