// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CAssetManager.generated.h"

class UCData;

/**
 * Game implementation of asset manager, overrides functionality and stores game-specific types
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic
 * This is used by setting AssetManagerClassName in DefaultEngine.ini - not impl(TODO)
 */

UCLASS()
class TACTICALROGUELITE_API UCAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	
	//Constructor and overrides.
	UCAssetManager() {}
	virtual void StartInitialLoading() override;

	/**Static types for items*/
	static const FPrimaryAssetType	BootsItemType;
	static const FPrimaryAssetType	WeaponItemType;
	static const FPrimaryAssetType	ArmorItemType;
	static const FPrimaryAssetType	TrinketItemType;
	static const FPrimaryAssetType	CharmItemType;

	static const FPrimaryAssetType	AchievementItemType;

	/** Returns the current AssetManager object */
	static UCAssetManager& Get();

	/**
	 * Synchronously loads an (item)Data subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load.
	 * @param bDisplayWarning If true, this will log a warning if the item failed to load.
	 */
	UCData* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
