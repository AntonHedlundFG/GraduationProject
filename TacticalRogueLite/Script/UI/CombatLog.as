
class USCombatLogWidget: UCResizableWindow
{
    const FString newline = "\n";
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UScrollBox ScrollBox;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta=(MultiLine=true))
    URichTextBlock TextBox;
    FString PrevMessage = "";
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UCGameplayLogDropDownMenu FilterMenu;
    int LogFilter;

    UFUNCTION(BlueprintCallable)
    void ClearLog()
    {
        TextBox.SetText(FText::FromString(""));
    }
    UFUNCTION()
    private void RecieveMessage(ELogCategory Category,const FString& Message)
    {
        int Flag = UCGameplayLogDropDownMenu::GetFilterFlag(Category);
        if(LogFilter & Flag == Flag)
        {
            AddMessage(Message);
        }
    }
    UFUNCTION(BlueprintCallable)
    void Initialize()
    {
        UCLogManager::Get().OnNewLogEntry.AddUFunction(this,n"RecieveMessage");
        FilterMenu.OnFilterUpdated.AddUFunction(this,n"UpdateFilter");
        //Since the log is initialized before the filter, We have to set the filter here, before it can be updated;
        LogFilter |= UCGameplayLogDropDownMenu::GetFilterFlag(ELogCategory::LC_Gameplay);
       // AddMessage(UCCombatLogger::Format(ECombatLogCategory::COMBAT,"Combat Logger Initialized."));
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
    UFUNCTION()
    void UpdateFilter(int Filter)
    {
        LogFilter = Filter;
    }

}
