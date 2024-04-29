class USCloserToValidUnitConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FGameplayTagContainer BlockingTags;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        // Get reference to Valid Units based on tags considerationtags
        TArray<ACUnit> Units = ConsiderationTags.HasTag(GameplayTags::Unit_IsPlayer) ? Context.PlayerUnits : Context.AIUnits;
        TArray<ACGridContent> ContentArray;

        for (ACUnit PlayerUnit : Units)
        {
            ContentArray.Add(PlayerUnit);
        }

        FGameplayTagContainer Tags = BlockingTags;
        ACUnit ClosestUnit = Cast<ACUnit>(CGridUtils::GetClosestGridContent(TargetTile, ContentArray, ConsiderationTags, BlockingTags));

        if (ClosestUnit != nullptr)
        {
            int DistanceFromStart = CGridUtils::AStar_Pathfinding(Context.CurrentUnit.GetTile(), ClosestUnit.GetTile(), ConsiderationTags, BlockingTags).Num();
            int DistanceFromTarget = CGridUtils::AStar_Pathfinding(TargetTile, ClosestUnit.GetTile(), ConsiderationTags, BlockingTags).Num();
            if(DistanceFromTarget == 0 || DistanceFromStart == 0) return Curve.GetFloatValue(1);
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