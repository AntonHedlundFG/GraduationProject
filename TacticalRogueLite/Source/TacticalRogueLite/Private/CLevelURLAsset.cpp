// Fill out your copyright notice in the Description page of Project Settings.


#include "CLevelURLAsset.h"

FString UCLevelURLAsset::URLOfLevel(const FLevelInfo& inLevelRef, FLevelLoadSettings inSettings)
{
	FString LoadPath = inLevelRef.LevelLoadPath;
	return LoadPath
		.Append(FString("?listen?port=7779"))
		.Append(inSettings.GetOptionsStringAppendage());
}

FString UCLevelURLAsset::URLOfLevelByIndex(const int inLevelIndex, FLevelLoadSettings inSettings)
{
	if (inLevelIndex < 0 || inLevelIndex >= Levels.Num())
		return FString();
	return URLOfLevel(Levels[inLevelIndex], inSettings);
}

FString UCLevelURLAsset::URLOfLevelByName(const FString& inName, FLevelLoadSettings inSettings)
{
	for (const FLevelInfo& Level : Levels)
	{
		if (Level.LevelName.Equals(inName))
			return URLOfLevel(Level, inSettings);
	}
	return FString();
}
