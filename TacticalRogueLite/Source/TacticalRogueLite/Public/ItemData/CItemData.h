//I use softobjectptr because it enables us to load parts of the asset using the assetmanager.
//https://dev.epicgames.com/documentation/en-us/unreal-engine/asset-management-in-unreal-engine

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Engine/DataAsset.h"
#include "CItemData.generated.h"

UENUM()
enum class EItemQuality : uint8
{
	Common,
	UnCommon,
	Rare,
	Epic,
	Legendary,
};

//Base class for anything considered an item in the game. Can be things like weapons, schematics, upgrades. Only contains the data for these items.
//PrimaryDataAsset adds the support to use AssetManager, which is a system that lets us more easly asynchronously load parts of assets.
//https://www.tomlooman.com/unreal-engine-asset-manager-async-loading/
UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// //Get item name.
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	// FText GetTitle() const;
	//
	// //Get item name, overriden in Blueprint to.
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	// FText GetDescription() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText ItenName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText ItemDescription;

public:

	
	//Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	//Access in blueprints via 'get as texture/material' helpers to allow easy soft-loading which can't be done through base class.
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AssetBundles = "UI", AllowedClasses = "MaterialInterface, Texture2D"))
	TSoftObjectPtr<UObject> Icon;

	//Weapon, augment etc. Used by the asset manager and game code may use it to understand the available 'features' of the item.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemQuality Quality;

	//Maximum allowed in the stack. 0 = unlimited.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 StackLimit;

	//Optional Action/Abilites assigned to Item. Can be used to grant abilities while the item is active/equipped or to run item specific functions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AssetBundles = "Actions"))
	TArray<TSoftClassPtr<UCAction>> ActionClasses;

	//Optional world representation of this object if dropped or equipped by a unit. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AssetBundles = "Actor"))
	TSoftClassPtr<AActor> ActorClass;

	//GamePlayTags to decorate the item. Can be used to setup categories, types, filters or one-off tags for very specific items.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tags")
	FGameplayTagContainer OwnedTag;

	//UCItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override;

public:

	//Utility to find any ItemData class through its OwnedTags without having to load every asset(using Asset Registry).
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
