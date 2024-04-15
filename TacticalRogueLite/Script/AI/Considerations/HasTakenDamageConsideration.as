class USTargetHealthPercentConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0.0f;

        TArray<ACGridTile> ValidTiles = UCAction::GetAbilityValidTargetTiles(Ability, StartTile);

        float AverageHealthPercentage = 0.0f;
        int NumUnits = 0;

        for(ACGridTile Tile : ValidTiles)
        {
            ACUnit Unit = Cast<ACUnit>(Tile.GetContent());
            if(!IsValid(Unit)) { continue; }
            float UnitHealthPercentage = Unit.GetAttributeComp().GetHealthPercent();
            AverageHealthPercentage += Curve.GetFloatValue(UnitHealthPercentage);
            NumUnits++;
        }   

        if(NumUnits == 0) { return 0.0f; }

        float FinalScore = AverageHealthPercentage / NumUnits;
        UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} - Average Health Percentage: {AverageHealthPercentage} - Num Units: {NumUnits}");

        return FinalScore;
    }


}