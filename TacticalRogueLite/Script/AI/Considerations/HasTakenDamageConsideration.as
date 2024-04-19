class USTargetHealthPercentConsideration : UCConsideration
{
    UPROPERTY(EditDefaultsOnly, Category = "Consideration")
    FRuntimeFloatCurve Curve;

    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
         float Score = 0.0f;

        ACUnit Unit = Cast<ACUnit>(TargetTile.GetContent());
        if(!IsValid(Unit)) 
        { 
            return 0; 
        }

        float UnitHealthPercentage = Unit.GetAttributeComp().GetHealthPercent();

        float FinalScore = Curve.GetFloatValue(UnitHealthPercentage);
        
        //UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} - Average Health Percentage: {AverageHealthPercentage} - Num Units: {NumUnits}");

        return FinalScore;
    }


}