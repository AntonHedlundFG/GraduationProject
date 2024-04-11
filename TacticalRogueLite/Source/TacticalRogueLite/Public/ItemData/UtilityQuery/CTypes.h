// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Curves/CurveFloat.h"
#include "Engine/DataTable.h"
#include "CTypes.generated.h"


class UCActionComponent;

/*
 * Item Table configuration for random loot, random events, maybe procedural levels..?
 * Optional: Derive from this to add your own table categories(Cant derive from in BP content, only C++).
 */
 
 USTRUCT(BlueprintType)
 struct FItemTableRow : public FTableRowBase
 {
 	GENERATED_BODY()

 public:

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemTable")
 	FPrimaryAssetId ItemId;

 	//Use -1 weight for guaranteed drop.
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemTable")
 	float Weight;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quantity")
 	int32 Min;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quantity")
 	int32 Max;

 	//Item is only included if all tags are present when rolling.
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
 	FGameplayTagContainer RequiredTags;

 	//Item is excluded when the roll includes this tag.
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
 	FGameplayTagContainer BlockedTags;

 	//Experimental. https://zhuanlan.zhihu.com/p/666639858. "A set of tags that can be queried to see if they meet specific conditions".
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Tags")
 	FGameplayTagQuery TagQuery;

 	//Optional Bucket to use when rolling. Groups togheter items and turn rolling into 2 phases (roll bucket, roll item within bucket).
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Advanced")
 	FName BucketName;

 	//Runtime toggle for rolling without replacement.
 	bool bIgnored;

 	//Total weight, can be zero if excluded from roll.
 	float GetItemWeight(const FGameplayTagContainer& ContextTags, const FName& InBucket)
 	{
 		if (bIgnored)
 		{
 			return 0.0f;
 		}
 		//Filter to Bucket.
 		if (!InBucket.IsEqual(BucketName, ENameCase::IgnoreCase))
 		{
 			return 0.0f;
 		}

 		if (Weight < 0.0f || !MatchesTags(ContextTags))
 		{
 			//Exclude guaranteed drops.
 			return 0.0f;
 		}
 		return Weight;
 	}

 	//Check against tags if any are specified.
 	bool MatchesTags(const FGameplayTagContainer& ContextTags)
 	{
 		if (ContextTags.HasAllExact(RequiredTags) && !ContextTags.HasAny(BlockedTags))
 		{
 			if (TagQuery.IsEmpty())
 			{
 				return true;
 			}
 			return TagQuery.Matches(ContextTags);
 		}

 		return false;
 	}

 	FItemTableRow()
 	{
 		Min = 1;
 		Max = 1;
 		Weight = 10;
 		BucketName = "Default";
 	}
 };

USTRUCT(BlueprintType)
struct FItemRollResult
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FPrimaryAssetId ItemID;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;
};

UENUM(BlueprintType)
enum class ERollType : uint8
{
	//When item is picked it stays in the bucket to be possible picked again.
	WithReplacement,

	//When picked the item is no longer considered to be picked again until next reset.
	WithoutReplacement
};

//Configures individual buckets at runtime before a roll on item table.
USTRUCT(BlueprintType)
struct FBucketInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BucketName;

	//Max times this bucket can be picked. Use 0 to ignore an entire Group for even more control.
	//(0=IgnoredBucket, -1=UnlimitedReplacement).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxOccurance;

protected:

	//Default weight to assign a bucket.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

public:
	FBucketInfo()
	{
		BucketName = "Default";
		MaxOccurance = 1;
		Weight = 10.0f;

		TimesSelected = 0;
	}

	//Internal use...

	//Remaining Bucket weight including replacement logic.
	int32 BucketWeight()
	{
		if (MaxOccurance <= -1)
		{
			return Weight;
		}

		if (TimesSelected < MaxOccurance)
		{
			return Weight;
		}

		return 0.0f;
	}
	int32 TimesSelected;
};

UCLASS()
class TACTICALROGUELITE_API UCTypes : public UDataTable
{
	GENERATED_BODY()
	
};
