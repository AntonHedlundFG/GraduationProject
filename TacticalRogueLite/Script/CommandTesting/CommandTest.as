class USTestAttackItem : UCItem
{
    UFUNCTION(BlueprintOverride)
    UCCommand GenerateAbilityCommand(AController inController, ACUnit inUnit, ACGridTile inTargetTile)
    {
        USTestAttackCommand NewCommand = Cast<USTestAttackCommand>(NewObject(inController, USTestAttackCommand::StaticClass()));
        NewCommand.Instigator = inUnit;
        NewCommand.Target = Cast<ACUnit>(inTargetTile.Content);

        return NewCommand;
    }

    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACUnit inUnit)
    {
        TArray<ACGridTile> Neighbours = inUnit.GetTile().GetNeighbours();
        TArray<ACGridTile> Attackables;
        for (ACGridTile Neighbour : Neighbours)
        {
            if (Neighbour.Content != nullptr && Neighbour.Content.IsA(ACUnit::StaticClass()))
            {
                Attackables.Add(Neighbour);
            }
        }
        return Attackables;
    }

}

class USTestAttackCommand : UCCommand
{
    ACUnit Instigator;
    ACUnit Target;


    UCAttributeComponent Attributes;

    UFUNCTION(BlueprintOverride)
    void ExecuteCommand()
    {
        Attributes = UCAttributeComponent::GetAttributes(Target);
        if (Attributes == nullptr) return;


        Attributes.ApplyHealthChange(Instigator, -50);
        Print(f"{Attributes.Health}");

    }
    UFUNCTION(BlueprintOverride)
    void UndoCommand()
    {
        if (Attributes == nullptr) return;

        Attributes.ApplyHealthChange(Instigator, 50);
    }
    UFUNCTION(BlueprintOverride)
    FString ToString()
    {
        //return f"{Instigator} is dealing damage to {Target}.";
       return UCCombatLogger::Format(ECombatLogCategory::COMBAT,f"{Instigator} is dealing damage to {Target}.");
    }
}

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
        TArray<ACGridTile> Tiles = inTile.GetNeighbours();
        for (int i = Tiles.Num() - 1; i >= 0; i--)
        {
            if (Tiles[i].GetContent() != nullptr)
                Tiles.RemoveAtSwap(i);
        }
        return Tiles;
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
        return UCCombatLogger::Format(ECombatLogCategory::DEFAULT,f"Moving {TargetUnit = } from {FromTile = } to {TargetTile = }");
    }
}