// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlay/Subsystems/CItemRollingSubSystem.h"

#include "CGameState.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ItemData/CItemData.h"
#include "Utility/CRandomComponent.h"
#include "Utility/Logging/CLogManager.h"

void UCItemRollingSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//Fetch all items through Asset Manager.
	if (UAssetManager* Manager = UCAssetManager::GetIfInitialized())
	{
		TArray<FPrimaryAssetId> AssetList;
		const FPrimaryAssetType AssetType("Item");

		Manager->GetPrimaryAssetIdList(AssetType, AssetList);

		TArray<FName> Bundles;
		Bundles.Add("UI");

		//Different syntax, creating a delegate via CreateUObject and passing
		//in the parameters we want to use once loading has completed several frames or seconds later.
		//In this case the AssetList. 
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
			this, &UCItemRollingSubSystem::OnItemsLoaded, AssetList);

		//Requests the load in Asset Manager on the AssetList and passes in the Delegate we just created.
		Manager->LoadPrimaryAssets(AssetList, Bundles, Delegate);
	}
}

void UCItemRollingSubSystem::OnItemsLoaded(TArray<FPrimaryAssetId> LoadedAssets)
{
	if (UAssetManager* Manager = UCAssetManager::GetIfInitialized())
	{
		for (FPrimaryAssetId Id : LoadedAssets)
		{
			UCItemData* Item = Cast<UCItemData>(Manager->GetPrimaryAssetObject(Id));
			if (Item == nullptr)
			{
				continue;
			}
			LoadedItemDatas.Add(Id, Item);
		}
	}
}

bool UCItemRollingSubSystem::RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results,
FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds,
TArray<FBucketInfo> Buckets,
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

		for (FBucketInfo& Bucket : Buckets)
		{
			TotalWeight += Bucket.BucketWeight();
		}
		
		float SelectedWeight = GetRand(0.0f, TotalWeight);
		FName SelectedBucket = "Default";

		for (FBucketInfo& Bucket : Buckets)
		{
			if(Bucket.Ignored())
			{
				continue;
			}
			CurrWeight += Bucket.BucketWeight();
			
			if (SelectedWeight <= CurrWeight)
			{
				SelectedBucket = Bucket.BucketName;
				Bucket.TimesSelected++;
				break;
			}
		}

		//Re-calc weight each roll as they may change.
		TotalWeight = 0;
		for (FItemTableRow* Row : Items)
		{
			if (!Row->ItemId.IsValid() || !Row->MatchesTags(ContextTags) || ExcludedIds.Contains(Row->ItemId))
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
			if (!Row->ItemId.IsValid() || !Row->MatchesTags(ContextTags) || ExcludedIds.Contains(Row->ItemId) || Row->bIgnored)
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
	for (FBucketInfo& Bucket : Buckets)
	{
		Bucket.TimesSelected = 0;
	}

	return Results.Num() > 0;
}

int32 UCItemRollingSubSystem::GetRand(int32 Min, int32 Max)
{
	if (ACGameState* GS = GetWorld()->GetGameState<ACGameState>())
	{
		return GS->Random->GetRandRange(Min, Max, false);
	}
	
	LOG_WARNING("Cant find Gamestate, returning 0 in GetRand!");
	return 0;
}

UCItemData* UCItemRollingSubSystem::GetItem(const FPrimaryAssetId& ID)
{
	if (LoadedItemDatas.Contains(ID)) { return LoadedItemDatas[ID]; }
	return nullptr;
}

TArray<UCItemData> UCItemRollingSubSystem::RollItems(UDataTable* Table, FGameplayTagContainer ContextTags, TArray<FBucketInfo> BucketInfo, int RollAmount)
{
	TArray<FItemRollResult> RollResults;
	TArray<FPrimaryAssetId> ExcludeIDs; //TODO: ?
	if(BucketInfo.IsEmpty()) { BucketInfo.Add(FBucketInfo()); }
	
	RollItemTable(Table, RollResults, ContextTags, ExcludeIDs, BucketInfo, RollAmount, ERollType::WithoutReplacement);

	TArray<UCItemData*> ReturnItems;
	
	for(auto Result : RollResults)
	{
		UCItemData* Data = GetItem(Result.ItemID);
		if(Data)
		{
			ReturnItems.Add(Data);
		}
	}
	
	return ReturnItems;
}
