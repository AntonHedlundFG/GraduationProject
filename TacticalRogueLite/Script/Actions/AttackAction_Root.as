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

   
    UCActionComponent ActionComponent = UCActionComponent::Get(TargetTile.GetContent());

    check(ActionComponent != nullptr);
	
	// Check if player already has action class.
	if (ActionComponent.GetAction(ActionToGrant) != nullptr)
	
	{
		CLogManager::Log(ELogCategory::LC_Gameplay, f"Rootingeffect is already on the target unit.");
		return;
	}

    //Apply effect.
	ActionComponent.AddAction(TargetTile.GetContent(), ActionToGrant);
    CLogManager::Log(ELogCategory::LC_Gameplay, f"Added effect rooted to unit");
	
	}

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
         UCActionComponent ActionComponent = UCActionComponent::Get(TargetTile.GetContent());

        check(ActionComponent != nullptr);

        if (ActionComponent.GetAction(ActionToGrant) == nullptr)
        {
            CLogManager::Log(ELogCategory::LC_Gameplay, f"ActionToGrant is not in ActionComp");
            return;
        }
      
        ActionComponent.RemoveAction(ActionComponent.GetAction(ActionToGrant));
        CLogManager::Log(ELogCategory::LC_Gameplay, f"Successfully removed rooting effect");
       
    }
}



