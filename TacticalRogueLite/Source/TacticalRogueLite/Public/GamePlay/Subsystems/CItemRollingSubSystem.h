// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData/CItemData.h"
#include "ItemData/UtilityQuery/CTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CItemRollingSubSystem.generated.h"

//Simple hook with only the tag(s) that just unlocked, the UI for instance can easily fetch which Achievement(data) it belongs to and async load if required.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemsRolled, TArray<UCItemData*>, Items);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPicked, UCItemData*, Item);

UCLASS()
class TACTICALROGUELITE_API UCItemRollingSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	
	UPROPERTY()
	TMap<FPrimaryAssetId,UCItemData*> LoadedItemDatas;
	
	void OnItemsLoaded(TArray<FPrimaryAssetId> LoadedAssets);

	UFUNCTION(BlueprintCallable, Category = "Game|Random", meta = (DefaultToSelf = "SourceActor", AdvancedDisplay="Buckets", AutoCreateRefTerm="Buckets, ExcludedIds"))
	bool RollItemTable(UDataTable* Table, TArray<FItemRollResult>& Results, FGameplayTagContainer ContextTags, TArray<FPrimaryAssetId> ExcludedIds, TArray<FBucketInfo> Buckets, int32 RollAmount = 1, ERollType ReplacementType = ERollType::WithoutReplacement);

	int32 GetRand(int32 Min,int32 Max);
	UCItemData* GetItem(const FPrimaryAssetId& ID);
	
public:

	UFUNCTION(BlueprintPure, Category = "Items", meta =(DisplayName = "Roll Items"))
	TArray<UCItemData*> RollItems(UDataTable* Table, FGameplayTagContainer ContextTags, TArray<FBucketInfo> BucketInfo, int RollAmount);

	//GetAllUnits();
	

	UPROPERTY(BlueprintAssignable, Category = "Items")
	FOnItemsRolled OnItemsRolled;
	
};
