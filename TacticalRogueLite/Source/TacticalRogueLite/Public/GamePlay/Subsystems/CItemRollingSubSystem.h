// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "ItemData/UtilityQuery/CTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CItemRollingSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCItemRollingSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	ACGameState* GameState;
public:
	UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (DefaultToSelf = "SourceActor", AdvancedDisplay="Buckets", AutoCreateRefTerm="Buckets, ExcludedIds"))
	bool RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results, FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets, int32 RollAmount = 1, ERollType ReplacementType = ERollType::WithoutReplacement);
	int32 GetRand(int32 Min,int32 Max);
};
