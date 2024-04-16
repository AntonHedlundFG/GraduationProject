class USUnitInAbilityRangeConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        FGameplayTag ValidTargetTag = Ability.AbilityTags.HasTag(GameplayTags::AbilityType_Offensive) ? GameplayTags::Unit_IsPlayer : GameplayTags::Unit_IsEnemy;
           
        ACGridContent Content = TargetTile.GetContent();
        if(!IsValid(Content)) return 0;
        
        ACUnit Unit = Cast<ACUnit>(Content);
        if(!IsValid(Unit)) return 0;

        if(Unit.GetAttributeComp().ActiveGameplayTags.HasTag(ValidTargetTag))
        {
            return 1.0f;
        }
        

        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} unit in range");
        
        return 0;
    }
}