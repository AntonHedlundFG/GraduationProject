class USUnitInAbilityRangeConsideration : UCConsideration
{
    UPROPERTY(EditAnywhere)
    bool bConsiderFriendlyUnits = false;
    UPROPERTY(EditAnywhere)
    bool bConsiderEnemyUnits = true;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0.0f;
        TArray<ACGridTile> ValidTiles = UCAction::GetValidTargetTiles(Ability, TargetTile);

        // Add Player units if ability is offensive, otherwise add AI units
        TArray<ACUnit> Units = Ability.AbilityTags.HasTag(GameplayTags::AbilityType_Offensive) ? Context.PlayerUnits : Context.AIUnits;

        for (ACUnit Unit : Units)
        {
            ACGridTile UnitTile = Unit.GetTile();
            if (UnitTile == nullptr)
            {
                continue;
            }
            if(ValidTiles.Contains(UnitTile))
            {
                Score += 1.0f /Context.PlayerUnits.Num();
            }
        }

        float FinalScore = Curve.GetFloatValue(Score);

        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} unit in range");
        
        return Curve.GetFloatValue(Score);
    }
}