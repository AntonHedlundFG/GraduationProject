class USCloserToPlayerUnitConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        // Find Closest Player Unit
        TArray<ACGridContent> PlayerUnits;

        for (ACUnit PlayerUnit : Context.PlayerUnits)
        {
            PlayerUnits.Add(PlayerUnit);
        }
        FGameplayTagContainer AbilityTags = Ability.AbilityTags;
        FGameplayTagContainer BlockingTags = Ability.BlockingTags;
        ACUnit ClosestUnit = Cast<ACUnit>(CGridUtils::GetClosestGridContent(StartTile, PlayerUnits, AbilityTags, BlockingTags));

        if (ClosestUnit != nullptr)
        {
            int DistanceFromStart = CGridUtils::BFS_Pathfinding(Context.CurrentUnit.GetTile(), ClosestUnit.GetTile(), AbilityTags, BlockingTags, false).Num();
            int DistanceFromTarget = CGridUtils::BFS_Pathfinding(TargetTile, ClosestUnit.GetTile(), AbilityTags, BlockingTags, false).Num();
            if(DistanceFromTarget == 0 || DistanceFromTarget == 0) return 1;
            float DistanceScore = float(DistanceFromTarget) / float(DistanceFromStart);
            if(DistanceScore > 0 && DistanceScore < 1)
            {
                Score = 1 - DistanceScore;
            }
        }

        float FinalScore = Curve.GetFloatValue(Score);
        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} closer to player unit.");

        return FinalScore;
    }
}