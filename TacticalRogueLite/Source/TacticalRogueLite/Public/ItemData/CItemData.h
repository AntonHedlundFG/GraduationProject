//I use softobjectptr because it enables us to load parts of the asset using the assetmanager.
//https://dev.epicgames.com/documentation/en-us/unreal-engine/asset-management-in-unreal-engine

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Engine/DataAsset.h"
#include "CItemData.generated.h"

class ACUnit;

//Base class for anything considered an item in the game. Can be things like weapons, schematics, upgrades. Only contains the data for these items.
//PrimaryDataAsset adds the support to use AssetManager, which is a system that lets us more easly asynchronously load parts of assets.
//https://www.tomlooman.com/unreal-engine-asset-manager-async-loading/
UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetItemName() const { return ItemName; }
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetDescription() const { return ItemDescription; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FString ItemDescription;

public:

	/// <summary>
	/// Called by the server when a unit equips this item. This applies any
	/// tags in GrantsTags to the unit, and EquipOnUnitInternal can be overridden
	/// to apply further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	void EquipOnUnit(ACUnit* inUnit);

	/// <summary>
	/// Called by the server when a unit unequips this item. Removes any tags in 
	/// GrantsTags from the unit, and UnequipOnUnitInternal can be overridden to 
	/// remove any further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	void UnequipOnUnit(ACUnit* inUnit);

	
	//Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	//Access in blueprints via 'get as texture/material' helpers to allow easy soft-loading which can't be done through base class.
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AssetBundles = "UI", AllowedClasses = "MaterialInterface, Texture2D"))
	TSoftObjectPtr<UObject> Icon;

	// TODO: Figure out if this is useful
	// Weapon, augment etc. Used by the asset manager and game code may use it to understand the available 'features' of the item.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	//FPrimaryAssetType ItemType;

	//Optional Action/Abilites assigned to Item. Can be used to grant abilities while the item is active/equipped or to run item specific functions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AssetBundles = "Actions"))
	FAbility Ability;

	//GamePlayTags to decorate the item. Can be used to setup categories, types, filters or one-off tags for very specific items.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tag")
	FGameplayTag ItemSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tag")
	FGameplayTagContainer ItemTags;

	UFUNCTION(BlueprintPure)
	FGameplayTag GetItemSlotTag() const
	{
		return ItemSlot;
	}

	FPrimaryAssetId GetPrimaryAssetId() const override;


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
