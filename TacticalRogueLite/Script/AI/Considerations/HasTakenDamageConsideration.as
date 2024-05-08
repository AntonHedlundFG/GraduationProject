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

        FGameplayTag HealthTag = GameplayTags::Attribute_Health;
        FGameplayTag MaxHealthTag = GameplayTags::Attribute_HealthMax;
        
        int CurrentValue, BaseValue, Additive, Multiplier;
        int MaxCurrentValue, MaxBaseValue, MaxAdditive, MaxMultiplier;

        float FinalScore = 0;
        if(Unit.GetActionComp().GetAttribute(HealthTag, CurrentValue, BaseValue, Additive, Multiplier) &&
            Unit.GetActionComp().GetAttribute(MaxHealthTag, MaxCurrentValue, MaxBaseValue, MaxAdditive, MaxMultiplier))
        {
            float healthPercentage = float(BaseValue) / float(MaxBaseValue);
            FinalScore = Curve.GetFloatValue(healthPercentage);
            // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} - Average Health Percentage: {healthPercentage}");
        }
        

        return FinalScore;
    }


}