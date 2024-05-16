class USAction_ApplyActionOnArmorGain: UCAction
{
     
    UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCAction> ActionToGrant;

    FAttributeChangedSignature Signature;
    
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
            //Make sure an action class was set up.
	    if (!ensureAlways(ActionToGrant != nullptr))
	    {
		    return;
	    }

        UCActionComponent ActionComponent = GetOwningComponent();
        
        Signature.BindUFunction(this, n"OnArmorChange");
        
        ActionComponent.AddAttributeChangedListener(GameplayTags::Attribute_Armor, Signature, false);
    
    }

    UFUNCTION(BlueprintOverride)
    void StopAction(AActor Instigator)
    {
         UCActionComponent ActionComponent = GetOwningComponent();

         ActionComponent.RemoveAttributeChangedListener(GameplayTags::Attribute_Armor,Signature);
    }
    
    UFUNCTION()
    void OnArmorChange(UCActionComponent ActionComponent,
                                         UCActionComponent InstigatorComponent, FGameplayTag AttributeTag,
                                         int NewValue, int Delta,
                                         const FGameplayTagContainer&in ContextTags,
                                         EAttributeModifierOperation ModType)
    {
        if(ContextTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Action.Triggered"))) || Delta <= 0 )
        {
            return;
        }
        ActionComponent.AddAction(ActionComponent.GetOwner(), ActionToGrant);
    }
}