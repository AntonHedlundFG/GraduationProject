class USCloserToPlayerUnitConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        // Find Closest Player Unit
        FGameplayTagContainer MovementTags;
        MovementTags.AddTag(GameplayTags::Movement_Diagonal);
        MovementTags.AddTag(GameplayTags::Movement_Straight);
        TArray<ACGridContent> PlayerUnits;

        for (ACUnit PlayerUnit : Context.PlayerUnits)
        {
            PlayerUnits.Add(PlayerUnit);
        }

        ACUnit ClosestUnit = Cast<ACUnit>(CGridUtils::GetClosestGridContent(StartTile, PlayerUnits, MovementTags, MovementTags));

        if (ClosestUnit != nullptr)
        {
            int DistanceFromStart = CGridUtils::BFS_Pathfinding(Context.CurrentUnit.GetTile(), ClosestUnit.GetTile(), MovementTags, MovementTags).Num();
            int DistanceFromTarget = CGridUtils::BFS_Pathfinding(TargetTile, ClosestUnit.GetTile(), MovementTags, MovementTags).Num();
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