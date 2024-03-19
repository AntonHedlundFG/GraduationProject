class USTestItem : UCItem
{
    UPROPERTY()
    int TestValue = 2;

    UFUNCTION(BlueprintOverride)
    UCCommand GenerateAbilityCommand(AController inController, ACUnit inUnit, int inTileIndex)
    {
        USTestCommand NewCommand = Cast<USTestCommand>(NewObject(inController, USTestCommand::StaticClass()));
        NewCommand.TestInt = TestValue;
        return NewCommand;
    }
}

class USTestCommand : UCCommand
{
    int TestInt = 0;

    UFUNCTION(BlueprintOverride)
    void ExecuteCommand()
    {
        Print(f"Executing: {TestInt = }");
    }
    UFUNCTION(BlueprintOverride)
    void UndoCommand()
    {
        Print(f"Undoing: {TestInt = }");
    }
    UFUNCTION(BlueprintOverride)
    FString ToString()
    {
        return f"Test Command with value: {TestInt}";
    }
}

class ASTestUnit : ACUnit
{

}