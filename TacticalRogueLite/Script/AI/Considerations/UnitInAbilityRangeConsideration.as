class USUnitInAbilityRangeConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        TArray<ACGridTile> ValidTiles = UCAction::GetAbilityValidTargetTiles(Ability, StartTile);

        FGameplayTag ValidTargetTag = Ability.AbilityTags.HasTag(GameplayTags::AbilityType_Offensive) ? GameplayTags::Unit_IsPlayer : GameplayTags::Unit_IsEnemy;

        for(ACGridTile Tile : ValidTiles)
        {
            ACGridContent Content = Tile.GetContent();
            if(!IsValid(Content)) continue;
            
            ACUnit Unit = Cast<ACUnit>(Content);
            if(!IsValid(Unit)) continue;
            if(Unit.GetAttributeComp().ActiveGameplayTags.HasTag(ValidTargetTag))
            {
                return 1.0f;
            }
        }

        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} unit in range");
        
        return 0;
    }
}