// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Misc/Optional.h"
#include "CLevelURLAsset.generated.h"

#define NUMBER_OF_PLAYERS FString("PlayerCount")

/* This struct can be expanded to include more settings. 
* Make sure the settings are implemented just like NumberOfPlayers
* 1. Declare the UPROPERTY(). 
* 2. #define a macro for its name, it will make sure there are no spelling errors in FStrings. 
* 3. Update GetOptionsStringAppendage() to AddOption() the new property. It's good practice to not include un-assigned properties.
* After these steps are done, when you load a level with these settings, you can fetch the stored properties
* in the GameMode by using UGameplayStatics::GetIntOption(OptionsString, YOUR_MACRO, 0), and similar functions.
*/
USTRUCT(BlueprintType)
struct FLevelLoadSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumberOfPlayers = 0;

	//This string can be attached to the URL of the level you load to attach relevant settings.
	FString GetOptionsStringAppendage()
	{
		FString ReturnString;
		if (NumberOfPlayers > 0)
			AddOption(ReturnString, NUMBER_OF_PLAYERS, FString::FromInt(FMath::Clamp(NumberOfPlayers, 1, 4)));
		return ReturnString;
	}

private:

	void AddOption(FString& outOptionList, const FString& inOptionName, const FString& inValue = FString())
	{
		outOptionList.Append(FString("?") + inOptionName);
		if (!inValue.IsEmpty())
			outOptionList.Append(FString("=") + inValue);
	}

};

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

public:

	//Arbitrary name for level, which can then be found in the CLevelURLAsset by using URLOfLevelByName()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;

	//This should be the path of the level. These read like: "\Game\Levels\TestLevels\SomeLevel"
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LevelLoadPath;

};

/** Store all level references in this asset to make loading levels easier.
* This way, we don't have to write URL's manually when travelling between levels.
 */
UCLASS(Blueprintable)
class TACTICALROGUELITE_API UCLevelURLAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FString URLOfLevel(const FLevelInfo& inLevelRef, FLevelLoadSettings inSettings = FLevelLoadSettings());

	UFUNCTION(BlueprintCallable)
	FString URLOfLevelByIndex(const int inLevelIndex, FLevelLoadSettings inSettings = FLevelLoadSettings());

	UFUNCTION(BlueprintCallable)
	FString URLOfLevelByName(const FString& inName, FLevelLoadSettings inSettings = FLevelLoadSettings());

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Level Loading")
	TArray<FLevelInfo> Levels;

	
};
