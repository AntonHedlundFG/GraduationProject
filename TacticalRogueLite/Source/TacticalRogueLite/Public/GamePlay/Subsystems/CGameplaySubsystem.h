
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemData/UtilityQuery/CTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utility/Logging/CLogManager.h"
#include "CGameplaySubsystem.generated.h"

class UDataTable;

//History of random bools in a specific Category. (Filtered Randomness).
//See URL: http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter03_Advanced_Randomness_Techniques_for_Game_AI.pdf


USTRUCT()
struct FRandomHistoryBool
{
	GENERATED_BODY()

public:

	//ID, usually instanceName+Category to ensure each object and use has a unique history.
	FName FilterID;

	//Roll history to use for filtering.
	TArray<bool> History;

	bool ApplyFilterToBool(bool OriginalValue, FRandomStream& ReferenceStream)
	{
		if (History.Num() < 4)
		{
			History.Add(OriginalValue);
			return OriginalValue;
		}

		//This combo keeps the last 10 history.
		if (History.Num() > 20)
		{
			//Remove a bunch at once to avoid overhead of doing one every time we do RNG.
			History.RemoveAt(0, 10);
		}

		int32 MaxIndex = History.Num() -1;

		//Prevent sequence of 4 of the same value.
		if (History[MaxIndex] == OriginalValue &&
			History[MaxIndex - 1] == OriginalValue &&
			History[MaxIndex -2] == OriginalValue &&
			History[MaxIndex - 3] == OriginalValue)
		{

			//75% chance to flip the value.
			bool ChangedValue = ReferenceStream.RandHelper(100) > 75 ? OriginalValue : !OriginalValue;
			if (ChangedValue != OriginalValue)
			{
				LOG_INFO("Filteredrandom: Detected repeated pattern (4x same value, eg 1111 or 0000 in RNG. Changed result from '%d' to '%d'.");
			}
			History.Add(ChangedValue);
			return ChangedValue;
		}

		if (History.Num() < 7)
		{
			History.Add(OriginalValue);
			return OriginalValue;
		}

		//Prevent repeated patterns.
		if (OriginalValue == History[MaxIndex - 3] &&
			History[MaxIndex] == History[MaxIndex - 4] &&
			History[MaxIndex - 1] == History[MaxIndex - 5] &&
			History[MaxIndex - 2] == History[MaxIndex - 6])
		{
			bool ChangedValue = !OriginalValue;

			LOG_INFO("Filtered random: Detected repeated pattern(two equal patterns of 4 eg. 11001100 in RNG. Changed result");
			History.Add(ChangedValue);
			return ChangedValue;
		}

		if (History[MaxIndex] == OriginalValue &&
			History[MaxIndex - 1] == OriginalValue &&
			History[MaxIndex - 2] == OriginalValue &&
			History[MaxIndex - 3] != OriginalValue &&
			History[MaxIndex - 4] != OriginalValue &&
			History[MaxIndex - 5] != OriginalValue)
		{
			bool ChangedValue = !OriginalValue;
			LOG_INFO("FilteredRandom Detected repeated pattern (000111 or 111000 in RNG");
			History.Add(ChangedValue);
			return ChangedValue;
		}

		//No (offending) pattern found.
		History.Add(OriginalValue);
		return OriginalValue;
	}
};



//History of random floats (0.0-1.0) in a specific Category. (Filtered RandomNess)
// See URL: http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter03_Advanced_Randomness_Techniques_for_Game_AI.pdf
USTRUCT()
struct FRandomHistoryFloat
{
	GENERATED_BODY()

public:

	//ID, usually instanceName+Category to ensure each object and use has a unique history.
	FName FilterID;

	TArray<float> History;
	

	
	float ApplyFilterToFloat(float OriginalValue, FRandomStream& ReferenceStream)
	{
		//Only filters 0.0-1.0 range (for full range use "return InMin + (InMax - InMin) * RandomFloat* after filtering this value of 0.0-1.0) *???
		if (OriginalValue < 0.0f || OriginalValue > 1.0f)
		{
			return OriginalValue;
		}

		//This combo keeps the last 10 in history.
		if (History.Num() > 20)
		{
			//Remove a bunch at once to avoid overhead of doing one every time we do RNG.
			History.RemoveAt(0, 10);
		}
		if (CheckFilterRules(OriginalValue))
		{
			//Passed.
			History.Add(OriginalValue);
			return OriginalValue;
		}

		//Try at most 10 more times.
		for (int32 i = 0; i < 10; i++)
		{
			//Retry.
			float NewTry = ReferenceStream.FRand();
			if (CheckFilterRules(NewTry))
			{
				History.Add(NewTry);
				return NewTry;
			}
		}

		//Failed to find 'better' random number.
		return OriginalValue;
	}
	
	bool CheckFilterRules(float OriginalValue) //?
	{
		int32 MaxIndex = History.Num() - 1;

		if (MaxIndex < 1)
		{
			return true;
		}
		
		//Reroll if two consecutive numbers differ by less than 0.02, like 0.875 and 0.856.
		if (FMath::Abs(History[MaxIndex] - OriginalValue) <= 0.02f)
		{
			LOG_INFO("Filterrandom failed filter. Reason: too close to previous number");
			//Failed rule, request new number.
			return false;
		}

		if (MaxIndex < 2)
		{
			return true;
		}

		return true;

		//Reroll if three consecutive numbers differ by less than 0.1, like 0.345, 0.421 and 0.387.
		if (FMath::Abs(History[MaxIndex] - OriginalValue) < 0.1f &&
			FMath::Abs(History[MaxIndex - 1] - OriginalValue) < 0.1f &&
			FMath::Abs(History[MaxIndex] - History[MaxIndex - 1]) < 0.1f)
		{
			LOG_INFO("FilteredRandom: float failed filter. Reason: too similar to previous two");
			return false;
		}

		if (MaxIndex < 3)
		{
			return true;
		}

	//Optional extra rule: *Reroll if there is an incr4easing or decreasing run of 5 values, such as 0.342, 0.572, 0.619, 0.783 and 0.868.
	// ...

	const int32 IterateCount = 4;
	//Reroll if too many numbers at the top or bottom of the range within the last N values, such as 0.325, 0.198, 0.056, 0.432 and 0.216.
	if (OriginalValue > 0.5f)
	{
		for (int32 i = 0; i < IterateCount; i++)
		{
			if (History[MaxIndex - i] < 0.5f)
			{
				//Passed filter as soon as one value breaks the pattern.
				return true;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < IterateCount; i++)
		{
			if (History[MaxIndex - i] > 0.5f)
			{
				//Passed filter as soon as one value breaks the pattern.
				return true;
			}
		}
	}

	//Failed filter, the loops above early-out as soon as we pass.
	LOG_INFO("Failed filter, Reason: four consecutive numbers are all above or below 0.5");
	return false;
		
	}
};

//History of random int32 in a specific Category. (Filtered Randomness)
//See URL.
USTRUCT()
struct FRandomHistoryInt
{
	GENERATED_BODY()

	public:

	//ID, usually instanceNAme+Category to ensure each object and use has a unique history.
	FName FilterID;

	TArray<int32> History;

	//Stored min range, additional filtered ints must have the same min/max or it should use a new history.
	int32 Min;
	int32 Max;

	int32 ApplyFilterToInt(int32 OriginalValue, int32 InMin, int32 InMax, FRandomStream& ReferenceStream)
	{
		//Make sure we use the history consistently (min/max is sanity check as changing this per rng roll shouldnt fall in same history).
		if (History.Num() > 0)
		{
			if (Min != InMin)
			{
				return OriginalValue;
			}
			if (Max != InMax)
			{
				return OriginalValue;
			}
		}

		Min = InMin;
		Max = InMax;

		//This combo keeps the last 10 history.
		if (History.Num() > 20)
		{
			//Remove a bunch at once to avoid overhead of doing one every time we do RNG.
			History.RemoveAt(0, 10);
		}

		if(CheckFilterRules(OriginalValue))
		{
			//Passed.
			History.Add(OriginalValue);
			return OriginalValue;
		}

		//Try at most 10 more times.
		for (int32 i = 0; i < 10; i++)
		{
			//Retry.
			int32 NewTry = ReferenceStream.RandRange(InMin, InMax);
			if (CheckFilterRules(NewTry))
			{
				History.Add(NewTry);
				return NewTry;
			}
		}

		//Failed to find 'better' random number.
		return OriginalValue;
	}

	bool CheckFilterRules(int32 OriginalValue)
	{
		int32 MaxIndex = History.Num() - 1;

		if (MaxIndex < 1)
		{
			return true;
		}

		//Re-roll if we have the same number.
		if (OriginalValue == History[MaxIndex])
		{
			LOG_INFO("FilteredRandom: Rolled same int twice in a row. Retrying for..");
			return false;
		}

		if (MaxIndex < 2)
		{
			return true;
		}

		//Same value with one other inbetween, eg 8,3,8 or 5,7,5.
		if (OriginalValue == History[MaxIndex - 1])
		{
			LOG_INFO("FilteredRandom: Same int value rolled with only one other in between. Retrying for...");
			return false;
		}

		if (MaxIndex < 4)
		{
			return true;
		}

		//The middle of the range, if too many values are all below or above we should retry(eg range 0-10 and values 6,8,9,5,10).
		const int32 MiddleGround = (Min + Max) / 2.0f;

		if (OriginalValue > MiddleGround &&
		History[MaxIndex] > MiddleGround &&
		History[MaxIndex - 1] > MiddleGround &&
		History[MaxIndex - 2] > MiddleGround &&
		History[MaxIndex - 3] > MiddleGround)
		{
			LOG_INFO("FilteredRandom: 5 values all above the middle of range. Retrying...");
			return false;
		}

		if (OriginalValue < MiddleGround &&
		History[MaxIndex] < MiddleGround &&
		History[MaxIndex - 1] < MiddleGround &&
		History[MaxIndex - 2] < MiddleGround &&
		History[MaxIndex - 3] < MiddleGround)
		{
			LOG_INFO("FilteredRandom: 5 values all below the middle of range. Retrying...");
			return false;
		}

		//Passed remaining filters.
		return true;
	} 
};

//Storage for the filtered-random systems.
namespace RandomHelpers
{
	static FRandomStream RandomStream;

	//History of random bools, grouped per unique ID to allow for filtering.
	static TArray<FRandomHistoryBool> BoolHistory;

	//History of random floats, grouped per unique ID to allow for filtering.
	static TArray<FRandomHistoryFloat> FloatHistory;

	//History of random integers, grouped per unique ID to allow for filtering.
	static TArray<FRandomHistoryInt> IntHistory;
}


UCLASS()
class TACTICALROGUELITE_API UCGameplaySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	
	// //https://benui.ca/unreal/ufunction/
	// UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (DefaultToSelf = "SourceActor", AdvancedDisplay="Buckets", AutoCreateRefTerm="Buckets, ExcludedIds"))
	// bool RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results, FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets, int32 RollAmount = 1, ERollType ReplacementType = ERollType::WithoutReplacement);
	//
	// //RNG helper-Modify an Amount by percentage luck (eg. amount = 100, % = 80, means output is between 80 and 100).
	// UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (AdvancedDisplay = "FilterID"))
	// static float ChanceAmount(float Amount = 1, float Percentage = 50.f, FName FilterID = NAME_None);
	//
	// //RNG helper-Flat out luck calculation of succeed/fail based on percentage 0-100.
	// UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (AdvancedDisplay = "FilterID"))
	// static bool ChanceLuck(float Percentage = 50.0f, FName FilterID = NAME_None);
	
	//Get a random int in range using a game global seed. This is intended for server-only generation.
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category = "Game|Random", meta =(AdvancedDisplay = "FilterID"))
	static int32 GetRandomInt(int32 Min, int32 Max = 1, FName FilterID = NAME_None);

	//Get a random float in range using a game global seed. This is intended for server-only generation.
	// UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category = "Game|Random", meta = (AdvancedDisplay = "FilterID"))
	// static float GetRandomFloat(float Min, float Max = 1, FName FilterID = NAME_None);
	
	// //Get a random float in range using a game global seed. Use FilterID to track history and filter out repeated patterns in rng. This is intended for server-only generation.
	// UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category = "Game|Random", meta = (AdvancedDisplay = "FilterID"))
	// static bool GetRandomBool(FName FilterID);
	//
	// //Generates a random number with a normal distribution. Median is the 'middle' and Variance is the range in positive and negative direction.
	// //Source originally from ProceduralFoliageTile - Uses Box-Muller transformation http://mathworld.wolfram.com/Box-MullerTransformation.html . 
	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game|Random", meta=(Keywords="Normal Distribution,Bellcurve"))
	// static float GetRandomGaussian(float Median = 0, float Variance = 1.0f);
	

};

