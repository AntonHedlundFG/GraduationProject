class USValidTargetsInRangeConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0;

        // Get reference to relevant units
        TArray<ACUnit> ValidUnits = ConsiderationTags.HasTag(GameplayTags::Unit_IsPlayer) ? Context.PlayerUnits : Context.AIUnits;

        FGameplayTagContainer AbilityTags = Ability.AbilityTags;
        FGameplayTagContainer BlockingTags = Ability.BlockingTags;

        auto TargetsTiles = UCAction::GetAbilityValidTargetTiles(Ability, TargetTile);

        for (ACGridTile Tile : TargetsTiles)
        {
            if (Tile == nullptr) continue;

            ACUnit TargetUnit = Cast<ACUnit>(Tile.GetContent());
            if (TargetUnit == nullptr) continue;

            if (!ValidUnits.Contains(TargetUnit)) continue;

            Score += (1.0f / TargetsTiles.Num());
        }

        float FinalScore = Curve.GetFloatValue(Score);
        
        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} ValidTargetsInRange.");

        return FinalScore;
    }
}