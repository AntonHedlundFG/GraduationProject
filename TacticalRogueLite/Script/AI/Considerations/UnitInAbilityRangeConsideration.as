class USUnitInAbilityRangeConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
           
        ACGridContent Content = TargetTile.GetContent();
        if(!IsValid(Content)) return 0;
        
        ACUnit Unit = Cast<ACUnit>(Content);
        if(!IsValid(Unit)) return 0;

        if(Unit.GetGameplayTags().HasAny(ConsiderationTags))
        {
            return 1.0f;
        }
        

        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} unit in range");
        
        return 0;
    }
}