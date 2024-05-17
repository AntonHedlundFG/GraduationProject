class USAction_ApplyAttributeOnLowHealth : UCAction
{
    UPROPERTY(EditAnywhere)
    bool bUseAbsoluteHealthThreshold = false;

    UPROPERTY(EditAnywhere,meta = (EditCondition = !"bUSeAbsoluteHealthThreshold", EditConditionHides), meta = (ClampMin = 0,ClampMax = 1,UIMin = 0, UIMax = 1))
    float HealthPercentageThreshold = 0.5;
    UPROPERTY(EditAnywhere,meta = (EditCondition = "bUSeAbsoluteHealthThreshold", EditConditionHides),meta = (ClampMin = 1,UIMin = 1))
    int AbsoluteHealthThreshold = 1;
    
    UPROPERTY(EditAnywhere)
    FAttributeModification OnHealthLowModification;

    FAttributeChangedSignature Signature;

    bool bHasAppliedMod = false;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        Signature.BindUFunction(this,n"OnHealthChange");
        UCActionComponent ActionComponent = GetActionComp();
        ActionComponent.AddAttributeChangedListener(GameplayTags::Attribute_Health,Signature,true);
    }
    UFUNCTION(BlueprintOverride)
    void StopAction(AActor Instigator)
    {
        UCActionComponent ActionComponent = GetActionComp();
        ActionComponent.RemoveAttributeChangedListener(GameplayTags::Attribute_Health,Signature);
    }

    UFUNCTION()
    void OnHealthChange(UCActionComponent ActionComponent,
                                         UCActionComponent InstigatorComponent, FGameplayTag AttributeTag,
                                         int NewValue, int Delta,
                                         const FGameplayTagContainer&in ContextTags,
                                         EAttributeModifierOperation ModType)
    {
        float Threshold;

        bool bShouldApplyAttribute = false;
        bool bShouldRemoveAttribute = false;

        if(bUseAbsoluteHealthThreshold)
        {
            if(NewValue <= AbsoluteHealthThreshold && !bHasAppliedMod)
            {
                bShouldApplyAttribute = true;
            }
            else if(NewValue > AbsoluteHealthThreshold && bHasAppliedMod)
            {
                bShouldRemoveAttribute = true;
            }
        }
        else
        {
            int Trash;
            int MaxHealth;
            ActionComponent.GetAttribute(GameplayTags::Attribute_HealthMax,MaxHealth,Trash,Trash,Trash);
            float HealthPercentage = float(NewValue)/float(MaxHealth);

            if(HealthPercentage <= HealthPercentageThreshold && !bHasAppliedMod)
            {
                bShouldApplyAttribute = true;
            }
            else if(HealthPercentage > HealthPercentageThreshold && bHasAppliedMod)
            {
                bShouldRemoveAttribute = true;
            }
        }


        if(bShouldApplyAttribute)
        {
            bHasAppliedMod = true;
            ActionComponent.ApplyAttributeChange(OnHealthLowModification,0);
        }
        else if(bShouldRemoveAttribute)
        {
            FAttributeModification NegatedMod = OnHealthLowModification;
            NegatedMod.Magnitude = -NegatedMod.Magnitude;
            bHasAppliedMod = false;
            ActionComponent.ApplyAttributeChange(NegatedMod,0);
        }
    }
}