class USHealAction_AOE : UCAction
{
    UPROPERTY()
    int Range = 1;


    // List of modifiers to apply to attributes when the action is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeModification> ModifiersAppliedToTarget;

    UPROPERTY()
    bool bCanHealSelf = true;

	ACUnit AttackingUnit; //TODO: Hide
    TArray<ACUnit> TargetsArray;
    private TArray<int> ModifiersTargetActualDeltas;

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        
         AttackingUnit = Cast<ACUnit>(Instigator);
        if(!IsValid(AttackingUnit))
            return;


        TSet<ACGridTile> TilesInRange = GetActionInfluencedTiles(AttackingUnit.GetTile());

        for (ACGridTile Tile : TilesInRange)
        {
            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit TargetUnit = Cast<ACUnit>(Content);

            FGameplayTag Tag = TargetUnit.GetTeam();
            if (IsValid(TargetUnit) && Tag.MatchesTag(GameplayTags::Unit_IsPlayer))
            {
                if(TargetUnit == AttackingUnit && !bCanHealSelf)
                    continue;
                TargetsArray.Add(TargetUnit);
            }
        }

         if(TargetsArray.Num() < 0)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Error, "No targets in Targetsarray");
            return;
        }
        
        for (int i = 0; i < TargetsArray.Num(); i++)
        {
            for (FAttributeModification& Mod : ModifiersAppliedToTarget)
            {
                UCActionComponent TargetActionComp = TargetsArray[i].GetActionComp();
	            if (TargetActionComp == nullptr) continue;

                Mod.InstigatorComp = TargetActionComp;
                int ActualDelta = TargetActionComp.ApplyAttributeChange(Mod, 0);
                ModifiersTargetActualDeltas.Add(ActualDelta);
                UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} healed {TargetsArray[i].GetUnitName()} for <Green> {ActualDelta} </> health.");
            }
        }  
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
         for (int i = TargetsArray.Num() - 1; i >= 0; i--)
        {
		    for (int j = ModifiersAppliedToTarget.Num() - 1; j >= 0; j--)
		    {
                UCActionComponent TargetActionComp = TargetsArray[i].GetActionComp();
	            if (TargetActionComp == nullptr) continue;
                
			    FAttributeModification Mod = ModifiersAppliedToTarget[j];
			    Mod.InstigatorComp = TargetActionComp;
			    Mod.bIsUndo = true;
                int ActualDelta = ModifiersTargetActualDeltas.Last();
                ModifiersTargetActualDeltas.RemoveAt(ModifiersTargetActualDeltas.Num() - 1);
			    Mod.Magnitude = -ActualDelta;
			    TargetActionComp.ApplyAttributeChange(Mod, 0);

                UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} undid <Green> {Mod.Magnitude} </> healing on {TargetsArray[i].GetUnitName()}.");
           }
		}
	}
                
    UFUNCTION(BlueprintOverride)
    TSet<ACGridTile> GetActionInfluencedTiles(ACGridTile fromTile)
    {
        return CGridUtils::FloodFill(fromTile, Range, ActionTags, FGameplayTagContainer());
    }
}
