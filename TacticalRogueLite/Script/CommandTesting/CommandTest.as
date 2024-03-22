class USTestItem : UCItem
{
    UPROPERTY()
    int TestValue = 2;

    UFUNCTION(BlueprintOverride)
    UCCommand GenerateAbilityCommand(AController inController, ACUnit inUnit, ACGridTile inTargetTile)
    {
        USTestCommand NewCommand = Cast<USTestCommand>(NewObject(inController, USTestCommand::StaticClass()));
        NewCommand.TargetUnit = inUnit;
        NewCommand.TargetTile = inTargetTile;
        NewCommand.FromTile = inUnit.GetTile();

        return NewCommand;
    }

    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACUnit inUnit)
    {
        TSet<ACGridTile> Tiles = CGridUtils::FloodFill(this, inUnit.GetTile(), TestValue);
        TArray<ACGridTile> ReturnTiles;
        for (auto Tile : Tiles)
        {
            ReturnTiles.Add(Tile);
        }
        return ReturnTiles;
    }

    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetReachableTiles(ACGridTile inTile)
    {
        return inTile.GetNeighbours();
    }

}

class USTestCommand : UCCommand
{
    ACUnit TargetUnit;
    ACGridTile TargetTile;
    ACGridTile FromTile;

    UFUNCTION(BlueprintOverride)
    void ExecuteCommand()
    {
        TargetUnit.SetTile(TargetTile);
    }
    UFUNCTION(BlueprintOverride)
    void UndoCommand()
    {
        TargetUnit.SetTile(FromTile);
    }
    UFUNCTION(BlueprintOverride)
    FString ToString()
    {
        return f"Moving {TargetUnit = } from {FromTile = } to {TargetTile = }";
    }
}

class ASTestUnit : ACUnit
{

}