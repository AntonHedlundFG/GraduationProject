class USAttackAction_Root : UCAttackAction
{
    
    UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCAction> ActionToGrant;


    UFUNCTION(BlueprintOverride) //? Super???
    void StartAction(AActor Instigator)
    {

    //Make sure an action class was set up.
	if (!ensureAlways(ActionToGrant != nullptr))
	{
		return;
	}

   
    UCActionComponent ActionComp = UCActionComponent::Get(TargetTile.GetContent());

    check(ActionComp != nullptr);
	
	// Check if player already has action class.
	if (ActionComp.GetAction(ActionToGrant) != nullptr)
	
	{
		UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "Rootingeffect is already on the target unit.");
		return;
	}

    //Apply effect.
	ActionComp.AddAction(TargetTile.GetContent(), ActionToGrant);
    UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "Added effect rooted to unit");
	
	}

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
         UCActionComponent ActionComp = UCActionComponent::Get(TargetTile.GetContent());

        check(ActionComp != nullptr);

        if (ActionComp.GetAction(ActionToGrant) == nullptr)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "ActionToGrant is not in ActionComp");
            return;
        }
      
        ActionComp.RemoveAction(ActionComp.GetAction(ActionToGrant));
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "Successfully removed rooting effect");
       
    }
}



