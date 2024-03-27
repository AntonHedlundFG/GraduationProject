
class USCombatLogWidget: UUserWidget
{
    const FString newline = "\n";
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UScrollBox ScrollBox;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta=(MultiLine=true))
    URichTextBlock TextBox;
    FString PrevMessage = "";
    
    UFUNCTION(BlueprintCallable)
    void ClearLog()
    {
        TextBox.SetText(FText::FromString(""));
    }
    UFUNCTION()
    private void RecieveMessage(ELogCategory Category,const FString& Message)
    {
        if(Category == ELogCategory::LC_Gameplay)
        {
            AddMessage(Message);
        }
    }
    UFUNCTION(BlueprintCallable)
    void Initialize()
    {
        UCLogManager::Get().OnNewLogEntry.AddUFunction(this,n"RecieveMessage"); 
        AddMessage(UCCombatLogger::Format(ECombatLogCategory::COMBAT,"Combat Logger Initialized."));
    }

    UFUNCTION(BlueprintCallable)
    void AddMessage(FString msg)
    {
        FString newMsg = PrevMessage + newline + msg;
        PrevMessage = newMsg;
        FText newText = FText::FromString(newMsg);
        TextBox.SetText(newText);
        ScrollBox.ScrollToEnd();
    }

}
