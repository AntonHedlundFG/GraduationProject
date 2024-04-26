// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Subsystems/CItemRollingSubSystem.h"
#include "Utility/CRandomComponent.h"
#include "Utility/Logging/CLogManager.h"

bool UCItemRollingSubSystem::RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results,
                                           FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets,
                                           int32 RollAmount, ERollType ReplacementType)
{
	if (Table == nullptr)
	{
		LOG_INFO("Rolling for items on a nullptr table");
		return false;
	}

	TArray<FItemTableRow*> Items;
	FString LogContext = "RollDice";
	Table->GetAllRows(LogContext, Items);
	ensureAlways(Items.Num() > 0);
	//The filtered random id to avoid rolling numbers too similar in sequence.
	//const FName FilterID = Table->GetFName();
	
	//First pass to handle all "always drop" items (does not consume RollAmount).
	for (FItemTableRow* Row : Items)
	{
		//ItemId may be null by design, less than 0 weight means guaranteed drop.
		if (Row->Weight >= 0.0f || !Row->ItemId.IsValid() || !Row->MatchesTags(ContextTags) || ExcludedIds.Contains(Row->ItemId))
		{
			continue;
		}

		FItemRollResult Item;
		Item.Amount = GetRand(Row->Min, Row->Max);
		Item.ItemID = Row->ItemId;
		Results.Add(Item);
	}

	if (RollAmount <= 0)
	{
		//Guaranteed items may still be picked.
		return Results.Num() > 0;
	}

	for (int32 i = 0; i < RollAmount; i++)
	{
		float TotalWeight = 0;
		float CurrWeight = 0;
		float SelectedWeight = GetRand(0.0f, TotalWeight);
		FName SelectedBucket = "Default";

		for (FBucketInfo& Bucket : Buckets)
		{
			CurrWeight += Bucket.BucketWeight();
			if (SelectedWeight <= CurrWeight)
			{
				SelectedBucket = Bucket.BucketName;
			}
		}

		//Re-calc weight each roll as they may change.
		TotalWeight = 0;
		for (FItemTableRow* Row : Items)
		{
			if (ExcludedIds.Contains(Row->ItemId))
			{
				//Skip excluded item.
				continue;
			}

			//Ignores the guaranteed drops with < 0.0f weight.
			TotalWeight += Row->GetItemWeight(ContextTags, SelectedBucket);
		}

		//No more items available (maybe from w/o replacement).
		if (TotalWeight == 0)
		{
			continue;
		}

		SelectedWeight = GetRand(0.0f, TotalWeight);
		CurrWeight = 0;
		for (FItemTableRow* Row : Items)
		{
			if (ExcludedIds.Contains(Row->ItemId))
			{
				//Skip excluded item.
				continue;
			}

			CurrWeight += Row->GetItemWeight(ContextTags, SelectedBucket);
			if (SelectedWeight <= CurrWeight)
			{
				//Item can be "null" which means nothing dropped by design.
				if (Row->ItemId.IsValid())
				{
					//Avoid negative values due to variance+median combinations.
					float Amount = GetRand(Row->Min, Row->Max);
					Amount = FMath::Max(1.0f, Amount);

					FItemRollResult Item;
					Item.Amount = Amount;
					Item.ItemID = Row->ItemId;
					Results.Add(Item);

					if (ReplacementType == ERollType::WithoutReplacement)
					{
						Row->bIgnored = true;
					}
				}
				//Found item.
				break;
			}
		}
	}
	//Make sure we RESET all replacement toggles.
	for (FItemTableRow* Row : Items)
	{
		Row->bIgnored = false;
	}

	return Results.Num() > 0;
}

int32 UCItemRollingSubSystem::GetRand(int32 Min, int32 Max)
{
	if(!GameState)
	{
		GameState = Cast<ACGameState>(GetWorld()->GetGameState());
		if(!GameState){return 0;}
	}
	return GameState->Random->GetRandRange(Min,Max,false);
}
