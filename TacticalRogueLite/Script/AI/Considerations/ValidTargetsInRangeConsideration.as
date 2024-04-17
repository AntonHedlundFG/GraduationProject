class USValidTargetsInRangeConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        // Get reference to relevant units

        auto TargetTiles = UCAction::GetAbilityInfluencedTiles(Ability, TargetTile);

        for (ACGridTile Tile : TargetTiles)
        {
            if (Tile == nullptr) continue;
            ACGridContent Content = Tile.GetContent();
            ACUnit TargetUnit = Cast<ACUnit>(Tile.GetContent());
            if (TargetUnit == nullptr) continue;
            if(TargetUnit == Context.CurrentUnit) continue;

            if (!TargetUnit.ActionComp.ActiveGameplayTags.HasAny(ConsiderationTags)) continue;

            Score += (1.0f / TargetTiles.Num());
        }

        float FinalScore = Curve.GetFloatValue(Score);
        
        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} ValidTargetsInRange.");

        return FinalScore;
    }
}