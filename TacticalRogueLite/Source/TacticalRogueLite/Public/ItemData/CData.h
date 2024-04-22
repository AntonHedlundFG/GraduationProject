// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Assets/AssetManager/CAssetManager.h"
#include "CData.generated.h"

class UCAction;

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Common,

	Uncommon,

	Rare,

	Epic,

	Legendary,
	
};

//Base class for anything considered an item in the game. Can be things like weapons, schematics, upgrades. Only contains the data for these items.
//PrimaryDataAsset adds the support to use AssetManager, which is a system that lets us more easly asynchronously load parts of assets.
//https://www.tomlooman.com/unreal-engine-asset-manager-async-loading/

UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	// //Get item name.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	FText GetTitle() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	FText GetDescription() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText ItemDescription;

public:
	
	//Access in blueprints via 'get as texture/material' helpers to allow easy soft-loading which can't be done through base class.
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AssetBundles = "UI", AllowedClasses = "MaterialInterface, Texture2D"))
	TSoftObjectPtr<UObject> Icon;

	//Boots, Weapon, Achievement etc. Used by the Asset Manager and game code may use it to understand the available "features" of the item.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	EItemQuality Quality;

	// Maximum amount allowed in the stack. 0 = unlimited.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	int32 StackLimit;

	//Actions assigned to Item. Can be used to grant abilities(in ItemData) or actions when this item is active/equipped or to simply run item specific functions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionsToGrantInstantlyWhenEquipped", meta = (AssetBundles = "Actions"))
	TArray<TSoftClassPtr<UCAction>> ActionClasses;

	//Tags to decorate the item. Can be used to set up categories, types, filters or one-off tags for very specific items. (Asset Registry Searchable).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Data")
	FGameplayTagContainer OwnedTags;
	
	UCData() {};
	

	FPrimaryAssetId GetPrimaryAssetId() const
	{
		// This is a DataAsset and not a blueprint so we can just use the raw FName
		// For blueprints you need to handle stripping the _C suffix.
		return FPrimaryAssetId(ItemType, GetFName());
	}

	//Utility to find any Data class through its OwnedTags without having to load every asset(using Asset Registry).
	// UFUNCTION(BlueprintCallable, Category = "AssetRegistry")
	// static FPrimaryAssetId GetItemDataByTag(FGameplayTagContainer InTags);

	//Utility to 'cast' to expected type(helpful for Blueprint that may expect explicit type rather than UObject base).
	UFUNCTION(BlueprintCallable, Category = "UI")
	TSoftObjectPtr<UMaterialInterface> GetIconAsMaterial() const
	{
		return TSoftObjectPtr<UMaterialInterface>(Icon.ToSoftObjectPath());
	}

	//Utility to 'cast' to expected type(helpful for Blueprint that may expect explicit type rather than Uobject base)
	UFUNCTION(BlueprintCallable, Category = "UI")
	TSoftObjectPtr<UTexture2D> GetIconAsTexture() const
	{
		return TSoftObjectPtr<UTexture2D>(Icon.ToSoftObjectPath());
	}
	

	
	
	

	
	
	

	

	
	
	

	//Add additional tooltip information in content browser.
	//void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
};
