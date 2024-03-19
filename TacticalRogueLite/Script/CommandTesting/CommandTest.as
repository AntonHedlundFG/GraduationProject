class USTestItem : UCItem
{
    UFUNCTION(BlueprintOverride)
    UCCommand GenerateAbilityCommand(AController inController, ACUnit inUnit, int inTileIndex)
    {
        USTestCommand NewCommand = Cast<USTestCommand>(NewObject(inController, USTestCommand::StaticClass()));
        NewCommand.TestInt = 5;
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
}

class ASTestUnit : ACUnit
{
    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        auto CastState = Cast<ACGameState>(World.GameState);
        CastState.AddUnitToOrder(this);
    }
}