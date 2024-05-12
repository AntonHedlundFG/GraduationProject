class USAttackAction_ChainLightning : UCTargetableAction
{
    UPROPERTY()
    int BounceRange = 1;
    UPROPERTY()
    int BounceCount = 1;

	ACUnit AttackingUnit;
    TArray<ACUnit> UnitsHit;
	TArray<FAttributeModifications> ReturnModifications;
    int Damaged = 0;


    UFUNCTION(BlueprintOverride)
    TArray<ACGridTile> GetValidTargetTiles(ACGridTile inTile)
    {
        if (GetOwningComponent() == nullptr)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Error, "GetValidTargetTiles found no OwningComponent, cannot reach AttributeComponent");
            return TArray<ACGridTile>();	
        }

        int Current = 0, Base = 0, Add = 0, Multi = 0;
        GetOwningComponent().GetAttribute(FGameplayTag::RequestGameplayTag(n"Attribute.AttackRange"), Current, Base, Add, Multi);

        return GetTargetsInRange(inTile, Current);
    }

    UFUNCTION(BlueprintOverride)
    void StartAction(AActor Instigator)
    {
        AttackingUnit = Cast<ACUnit>(Instigator);
        ACUnit Target = Cast<ACUnit>(TargetTile.GetContent());
        if(Target == nullptr)
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Warning, "Target is not a unit for Chain Lightning");
            return;
        }

        DamageTarget(Target);

        for (int i = 0; i < BounceCount; i++)
        {
            if(!IsValid(Target)) break;
            
            TArray<ACGridTile> TilesInRange = GetTargetsInRange(Target.GetTile(), BounceRange);
            TArray<ACGridContent> UnitsInRange;

            for (ACGridTile Tile : TilesInRange)
            {
                ACGridContent Content = Tile.GetContent();
                if(Content == nullptr)
                    continue;

                ACUnit Unit = Cast<ACUnit>(Content);

                if (IsValid(Unit) && !UnitsHit.Contains(Unit))
                {
                    UnitsInRange.Add(Unit);
                }
            }

            Target = Cast<ACUnit>(CGridUtils::GetClosestGridContent(Target.GetTile(), UnitsInRange, ActionTags, FGameplayTagContainer()));
            if(IsValid(Target))
            {
                DamageTarget(Target);
                continue;
            }
        }
        
    }

    UFUNCTION(BlueprintOverride)
    void UndoAction(AActor Instigator)
    {
        for (int i = UnitsHit.Num() - 1; i >= 0; i--)
        {
           for (FAttributeModification& Mod : ReturnModifications[i].Modifications)
           {
                UnitsHit[i].ActionComp.ApplyAttributeChange(Mod, 0);
                UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, f"{AttackingUnit.GetUnitName()} undid <Red> {Mod.Magnitude} </> damage on {UnitsHit[i].GetUnitName()}.");
           }
        }
    }

    TArray<ACGridTile> GetTargetsInRange(ACGridTile inTile, int Range)
    {
        TSet<FVector2D> TilesInRange = CGridUtils::FloodFillWithCoordinates(inTile.GetGridCoords(), Range, ActionTags);

        ACUnit Owner = Cast<ACUnit>(GetOwningComponent().GetOwner());
        if(!IsValid(Owner))
        {
            UCLogManager::BlueprintLog(ELogCategory::LC_Error, "GetTargetsInRange found no Owner, cannot reach AttributeComponent");
            return TArray<ACGridTile>();
        }

        // Filter invalid tiles
        FGameplayTag OpponentTag = Owner.GetGameplayTags().HasTag(GameplayTags::Unit_IsPlayer) ? (GameplayTags::Unit_IsEnemy) : (GameplayTags::Unit_IsPlayer);
        TArray<ACGridTile> ReturnTiles;
        for (FVector2D TileCoords : TilesInRange)
        {
            ACGridTile Tile = inTile.GetParentGrid().GetTileFromCoords(TileCoords);
            if(Tile == nullptr)
                continue;

            ACGridContent Content = Tile.GetContent();
            if(Content == nullptr)
                continue;

            ACUnit Unit = Cast<ACUnit>(Content);

            if (IsValid(Unit) && Unit.GetGameplayTags().HasTag(OpponentTag))
            {
                ReturnTiles.Add(Tile);
            }
        }
        ReturnTiles.Remove(inTile);

        return ReturnTiles;
    }

    void DamageTarget( ACUnit Target )
    {
        int Current = 0, Base = 0, Add = 0, Multi = 0;
        GetOwningComponent().GetAttribute(FGameplayTag::RequestGameplayTag(n"Attribute.AttackDamage"), Current, Base, Add, Multi);
        int Damage = Math::Max(1, Current - Damaged);
        Damaged++;
        FAttributeModifications Modifications = CGameplay::ApplyDamage(AttackingUnit, Target, Base, ActionTags); 
        ReturnModifications.Add(Modifications);
        UnitsHit.Add(Target);
        UCLogManager::BlueprintLog(ELogCategory::LC_Gameplay, "Chain Lightning hit " + Target.GetName());
    }


}