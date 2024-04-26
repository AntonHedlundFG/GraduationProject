// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameState.h"
#include "Assets/AssetManager/CAssetManager.h"
#include "ItemData/CItemData.h"
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
	UPROPERTY()
	TMap<FPrimaryAssetId,UCItemData*> LoadedItemDatas;
	UPROPERTY()
	ACGameState* GameState;
	UPROPERTY()
	UAssetManager* Manager;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void OnItemsLoaded(TArray<FPrimaryAssetId> LoadedAssets);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (DefaultToSelf = "SourceActor", AdvancedDisplay="Buckets", AutoCreateRefTerm="Buckets, ExcludedIds"))
	bool RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results, FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets, int32 RollAmount = 1, ERollType ReplacementType = ERollType::WithoutReplacement);
	int32 GetRand(int32 Min,int32 Max);
	UCItemData* GetItem(const FPrimaryAssetId& ID);
};
