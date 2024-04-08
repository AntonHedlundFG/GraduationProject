// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Assets/AssetManager/CAssetManager.h"
#include "CData.generated.h"


class UCAction;

UCLASS(Abstract, BlueprintType)
class TACTICALROGUELITE_API UCData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UCData() {}

	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tag")
	FGameplayTagContainer OwnedTags;

	// 0 = unlimited.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	int32 StackLimit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AssetBundles = "Actions"))
	TArray<TSoftClassPtr<UCAction>> ActionClasses;
	
	FPrimaryAssetId GetPrimaryAssetId() const
	{
		// This is a DataAsset and not a blueprint so we can just use the raw FName
		// For blueprints you need to handle stripping the _C suffix.
		return FPrimaryAssetId(ItemType, GetFName());
	}
};
