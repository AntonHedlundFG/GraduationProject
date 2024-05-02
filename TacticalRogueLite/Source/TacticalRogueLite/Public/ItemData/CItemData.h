//I use softobjectptr because it enables us to load parts of the asset using the assetmanager.
//https://dev.epicgames.com/documentation/en-us/unreal-engine/asset-management-in-unreal-engine

#pragma once

#include "CoreMinimal.h"
#include "ItemData/CData.h"
#include "Actions/CAction.h"
#include "Engine/DataAsset.h"
#include "CItemData.generated.h"

class ACUnit;


UCLASS(BlueprintType)
class TACTICALROGUELITE_API UCItemData : public UCData
{
	GENERATED_BODY()

	UCItemData()
	{
		ItemType = UCAssetManager::ItemType;
	}

public:

	/// <summary>
	/// Called by the server when a unit equips this item. This applies any
	/// tags in GrantsTags to the unit, and EquipOnUnitInternal can be overridden
	/// to apply further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	virtual void EquipOnUnit(ACUnit* inUnit);

	/// <summary>
	/// Called by the server when a unit unequips this item. Removes any tags in 
	/// GrantsTags from the unit, and UnequipOnUnitInternal can be overridden to 
	/// remove any further custom effects.
	/// </summary>
	/// <param name="inUnit"></param>
	virtual void UnequipOnUnit(ACUnit* inUnit);
	

	//Optional Action/Abilites assigned to Item. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AssetBundles = "Abilities"))
	FAbility Ability;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "ItemSlot", meta=(Categories="ItemSlot"), meta = (AssetBundles = "UI"))
	FGameplayTag ItemSlot;
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetItemSlotTag() const
	{
		return ItemSlot;
	}
};
