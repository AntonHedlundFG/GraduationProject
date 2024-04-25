

// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlay/Subsystems/CGameplaySubsystem.h"
#include "DSP/PassiveFilter.h"
#include "ItemData/UtilityQuery/CTypes.h"
#include "Utility/Logging/CLogManager.h"

//DECLARE_CYCLE_STAT(TEXT("GetRandomBool"), STAT_GetRandomBool, STATGROUP_LODZERO);

using namespace RandomHelpers;


void UCGameplaySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//TODO: In the original impl using GameState we initialized with a pre-set seed that was serialized during SaveGame.
	//May want this same behavior again once I have fully serialized the game.
	RandomStream = FRandomStream::FRandomStream(FMath::Rand());
}
/*
bool UCGameplaySubsystem::RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results,
                                        FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets, int32 RollAmount, ERollType ReplacementType)
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
	const FName FilterID = Table->GetFName();

	//First pass to handle all "always drop" items (does not consume RollAmount).
	for (FItemTableRow* Row : Items)
	{
		//ItemId may be null by design, less than 0 weight means guaranteed drop.
		if (Row->Weight >= 0.0f || !Row->ItemId.IsValid() || !Row->MatchesTags(ContextTags) || ExcludedIds.Contains(Row->ItemId))
		{
			continue;
		}

		FItemRollResult Item;
		Item.Amount = GetRandomInt(Row->Min, Row->Max, FilterID);
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
		float SelectedWeight = GetRandomFloat(0.0f, TotalWeight, FilterID);
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

		SelectedWeight = GetRandomFloat(0.0f, TotalWeight, FilterID);
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
					float Amount = GetRandomInt(Row->Min, Row->Max, FilterID);
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
	for ( FItemTableRow* Row : Items)
	{
		Row->bIgnored = false;
	}

	return Results.Num() > 0;
}


float UCGameplaySubsystem::ChanceAmount(float Amount, float Percentage, FName FilterID)
{
	if (Percentage >= 100.0f)
	{
		return Amount;
	}
	else if (Percentage > 0.0f)
	{
		float Range = 100 - Percentage;
		float Chance = GetRandomFloat(1.0f, Range, FilterID);
		Chance = (Chance + Percentage) / 100.0f;
		return Amount * Chance; //Applies the modifier.
	}
	return 0.0f;
}

bool UCGameplaySubsystem::ChanceLuck(float Percentage, FName FilterID)
{
	if (Percentage >= 100.0f)
	{
		return true;
	}
	else if (Percentage > 0.0f)
	{
		float Range = 100 - Percentage;
		//Can filter the random avoid repeat patterns.
		float Chance = GetRandomFloat(1, 100, FilterID);
		if (Chance > Range)
		{
			return true;
		}
	}
	return false;
}
*/

int32 UCGameplaySubsystem::GetRandomInt(int32 Min, int32 Max, FName FilterID)
{
	//SCOPE_CYCLE_COUNTER(STAT_GetRandomInt);

	int32 Value = RandomStream.RandRange(Min, Max);
	if (FilterID != NAME_None)
	{
		for (FRandomHistoryInt& HistoryStruct : IntHistory)
		{
			if (HistoryStruct.FilterID == FilterID)
			{
				return HistoryStruct.ApplyFilterToInt(Value, Min, Max, RandomStream);
			}
		}

		//Category doesnt exist yet, so create new history.
		FRandomHistoryInt NewStruct = FRandomHistoryInt();
		NewStruct.FilterID = FilterID;
		Value = NewStruct.ApplyFilterToInt(Value, Min, Max, RandomStream);
		IntHistory.Add(NewStruct);
	}

	return Value;
}

// float UCGameplaySubsystem::GetRandomFloat(float Min, float Max, FName FilterID)
//  {
// 	//SCOPE_CYCLE_COUNTER(STAT_GetRandomFloat);
//
//  	// 0.0 -1.0 Range.
//  	float Value = RandomStream.FRand();
//  	//Skip when no id was specified (
//  }






