class USAction_SecondWind : UCAction
{
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0"), meta = (ClampMax = "1"),meta = (UIMin = "0"),meta = (UIMax = "1"))
    float PercentualTreshold = 0.5;
    //UPROPERTY()
    ACGameState GameState;
   // UPROPERTY()
    ACUnit OwningUnit;
    UPROPERTY()
    FAttributeModification HealMod;
    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        GameState = Cast<ACGameState>(Gameplay::GetGameState());

       if(GameState != nullptr)
       {
            GameState.OnTurnOrderUpdate.AddUFunction(this,n"OnTurnOrderUpdate");
            OwningUnit = Cast<ACUnit>(GetOwningComponent().GetOwner());
       }
    }

    UFUNCTION(BlueprintOverride)
    void StopAction(AActor Instigator)
    {
        if(GameState == nullptr)
        GameState = Cast<ACGameState>(Gameplay::GetGameState());

       if(GameState != nullptr)
       {
            GameState.OnTurnOrderUpdate.AddUFunction(this,n"OnTurnOrderUpdate");
       }
    }
    UFUNCTION()
    void OnTurnOrderUpdate()
    {
        if(GameState.GetCurrentUnit() == OwningUnit)
        {
            int Trash;
            int MaxHealth = 0;
            int Health = 0;
            GetOwningComponent().GetAttribute(FGameplayTag::RequestGameplayTag(FName("Attribute.HealthMax")),MaxHealth,Trash,Trash,Trash);
            GetOwningComponent().GetAttribute(FGameplayTag::RequestGameplayTag(FName("Attribute.Health")),Health,Trash,Trash,Trash);

            float HealthPercentage = float(Health)/float(MaxHealth);

            if(HealthPercentage <= PercentualTreshold)
            {
               /* FAttributeModification mod;
                mod.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attribute.Health"));
                mod.ModifierOperation = EAttributeModifierOperation::AddBase;
                mod.InstigatorComp = GetOwningComponent();*/
                GetOwningComponent().ApplyAttributeChange(HealMod,0);
            }
        }
    }
}