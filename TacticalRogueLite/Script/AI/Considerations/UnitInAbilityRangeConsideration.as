class USUnitInAbilityRangeConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile, FCAIContext& Context)
    {
        float Score = 0.0f;
        TArray<ACGridTile> ValidTiles = UCAction::GetAbilityValidTargetTiles(Ability, TargetTile);

        // Add Player units if ability is offensive, otherwise add AI units
        TArray<ACUnit> Units = Ability.AbilityTags.HasTag(GameplayTags::AbilityType_Offensive) ? Context.PlayerUnits : Context.AIUnits;

        for (ACUnit Unit : Units)
        {
            ACGridTile UnitTile = Unit.GetTile();
            // If we find any unit on the valid tiles, return 1.0
            if(ValidTiles.Contains(UnitTile))
            {
                Score = 1.0f;
                break;
            }
        }

        // UCLogManager::BlueprintLog(ELogCategory::LC_Info, f"Final Score: {FinalScore} unit in range");
        
        return 0;
    }
}