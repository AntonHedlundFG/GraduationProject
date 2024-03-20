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

    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTiles(ACUnit inUnit)
    {
        TArray<ACGridTile> Tiles = inUnit.GetTile().GetNeighbours();
        return Tiles;
    }

}

class USTestCommand : UCCommand
{
    int TestInt = 0;

    UFUNCTION(BlueprintOverride)
    void ExecuteCommand()
    {
    }
    UFUNCTION(BlueprintOverride)
    void UndoCommand()
    {
    }
    UFUNCTION(BlueprintOverride)
    FString ToString()
    {
        return f"Test Command, Value: {TestInt}";
    }
}

class ASTestUnit : ACUnit
{

}