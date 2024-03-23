class USSavableComponent : UCSavableComponent
{
    UFUNCTION(BlueprintOverride)
    void OnSave() 
    {
        Print("USSavableComponent::OnSave not implemented, please Implement OnSave in your derived class");
    }

    UFUNCTION(BlueprintOverride)
    void OnLoad() 
    {
        Print("USSavableComponent::OnLoad not implemented, please Implement OnLoad in your derived class");
    }

}