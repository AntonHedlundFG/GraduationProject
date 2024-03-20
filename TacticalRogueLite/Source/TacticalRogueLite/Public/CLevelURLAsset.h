// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Misc/Optional.h"
#include "CLevelURLAsset.generated.h"

#define NUMBER_OF_PLAYERS FString("PlayerCount")

USTRUCT(BlueprintType)
struct FLevelLoadSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumberOfPlayers = 0;

	FString GetOptionsStringAppendage()
	{
		FString ReturnString;
		if (NumberOfPlayers > 0)
			AddOption(ReturnString, NUMBER_OF_PLAYERS, FString::FromInt(FMath::Clamp(NumberOfPlayers, 1, 4)));
		return ReturnString;
	}

private:

	void AddOption(FString& outOptionList, const FString& inOptionName, const FString& inValue)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LevelLoadPath;

};

/**
 * 
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
