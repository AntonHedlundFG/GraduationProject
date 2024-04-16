class USAlwaysTryUseConsideration : UCConsideration
{
    UFUNCTION(BlueprintOverride)
    float32 Evaluate(FAbility& Ability, ACGridTile StartTile, ACGridTile TargetTile,
                     FCAIContext& Context)
    {
        return 1.0f;
    }
}