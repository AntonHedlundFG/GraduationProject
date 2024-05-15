class USCloserToTargetConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FGameplayTagContainer BlockingTags;

    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FGameplayTagContainer TargetTags;


    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        TArray<ACGridTile> TileArray = GetAllTargetTiles(TargetTile, Context);

        if(TileArray.Num() == 0) 
        {
            return Curve.GetFloatValue(Score);
        }

        // Send along the start tile as well so that we can compare it to the target tiles
        TileArray.Add(StartTile);

        // Get the proximity score of all target tiles
        TMap<ACGridTile, float32> ProximityScoreMap;
        CGridUtils::GetProximityScore(TargetTile, TileArray, ConsiderationTags, BlockingTags, ProximityScoreMap);

        // Get the proximity score of the start tile
        float StartTileScore = ProximityScoreMap.Contains(StartTile) ? ProximityScoreMap[StartTile] : 0;

        // Find the best (highest) proximity score among the target tiles
        float BestTargetTileScore = 0;
        for (ACGridTile Tile : TileArray)
        {
            if (Tile != StartTile)
            {
                BestTargetTileScore = Math::Max(BestTargetTileScore, ProximityScoreMap.Contains(Tile) ? ProximityScoreMap[Tile] : 0);
            }
        }

        // Calculate the final score based on how much closer the start tile is compared to the best target tile
        if (BestTargetTileScore > 0)
        {
            // The closer the start tile is compared to the best target tile, the higher the score
            Score = 1 - (StartTileScore / (StartTileScore + BestTargetTileScore));
        }

        float FinalScore = Curve.GetFloatValue(Score);

        // CLogManager::Log(ELogCategory::LC_Info, f"Final Score: {FinalScore} closer to player unit.");

        return FinalScore;
    }

    TArray<ACGridTile> GetAllTargetTiles( ACGridTile FromTile , FCAIContext& Context) 
    {
        TArray<ACGridTile> Targets;
        TArray<ACUnit> Units = Context.AIUnits;
        Units.Append(Context.PlayerUnits);
        for (ACGridContent Unit : Units)
        {
            if (Unit.GetGameplayTags().HasAny(TargetTags))
            {
                Targets.Add(Unit.GetTile());
            }
        }

        return Targets;
    }
}