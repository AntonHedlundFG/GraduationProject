// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "CAchievementSubsystem.generated.h"

//Simple hook with only the tag(s) that just unlocked, the UI for instance can easily fetch which Achievement(data) it belongs to and async load if required.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, FGameplayTagContainer, AchievementTags);

UCLASS()
class TACTICALROGUELITE_API UCAchievementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	
	void OnAchievementsLoaded(TArray<FPrimaryAssetId> LoadedAssets);
	
public:
	
	//Unlock achievement by "Owned Tag".
	void UnlockAchievement(FGameplayTagContainer InTags);

	UFUNCTION(BlueprintCallable, Category = "Achievements")
	bool IsAchievementUnlocked(FGameplayTagContainer InTags);

	UPROPERTY(BlueprintAssignable, Category = "Achievements")
	FOnAchievementUnlocked OnAchievementUnlocked;
	
};
