class ASConsequenceUnit : ACUnit
{
    UPROPERTY()
    int Health = 2;

    void TakeDamage(int Damage)
    {
        int NewHealth = Health - Damage;
        Print(f"{NewHealth =}");
        if (Health > 0 && NewHealth < 1)
        {
            USDeathConsequence DeathConsequence = Cast<USDeathConsequence>(NewObject(this, USDeathConsequence::StaticClass()));
            if (DeathConsequence != nullptr)
            {
                DeathConsequence.DyingUnit = this;
                Cast<ACGameMode>(Gameplay::GameMode).RegisterAndExecuteConsequence(DeathConsequence);
            }
        }
        Health = NewHealth;
    }
}


class USDeathConsequence : UCConsequence
{
    ASConsequenceUnit DyingUnit;
    
    private int QueueIndex;
    private FVector DeathLocation;
    private ACGameState GameState;
    private ACGridTile Tile;

	UFUNCTION(BlueprintOverride)
	void UndoConsequence()
	{
        GameState = Cast<ACGameState>(Gameplay::GameState);
        if (GameState == nullptr) return;

        DyingUnit.ActorLocation = DeathLocation;

        if (QueueIndex >= 0)
            GameState.TurnOrder.Insert(DyingUnit, QueueIndex);

        if (Tile != nullptr)
        {
            Tile.SetContent(DyingUnit);
        }

	}

	UFUNCTION(BlueprintOverride)
	void ExecuteConsequence()
	{
        GameState = Cast<ACGameState>(Gameplay::GameState);
        if (GameState == nullptr) return;

        DeathLocation = DyingUnit.ActorLocation;
        DyingUnit.ActorLocation = FVector(10000, 10000, 10000);
        
        QueueIndex = GameState.TurnOrder.FindIndex(DyingUnit);
        if (QueueIndex >= 0)
            GameState.TurnOrder.RemoveAt(QueueIndex);

        Tile = DyingUnit.GetTile();
        if (Tile != nullptr)
        {
            Tile.SetContent(nullptr);
        }

	}

	UFUNCTION(BlueprintOverride)
	FString ToString()
	{
        return f"Unit {DyingUnit} died.";
	}
}