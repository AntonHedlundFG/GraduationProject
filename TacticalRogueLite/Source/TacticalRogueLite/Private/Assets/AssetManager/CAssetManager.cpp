// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/AssetManager/CAssetManager.h"
#include "ItemData/CData.h"
#include "Utility/Logging/CLogManager.h"

const FPrimaryAssetType	UCAssetManager::BootsItemType = TEXT("Boots");
const FPrimaryAssetType	UCAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType	UCAssetManager::ArmorItemType = TEXT("Armor");
const FPrimaryAssetType	UCAssetManager::TrinketItemType = TEXT("Trinket");
const FPrimaryAssetType	UCAssetManager::CharmItemType = TEXT("Charm");
const FPrimaryAssetType	UCAssetManager::AchievementItemType = TEXT("Achievement");
const FPrimaryAssetType	UCAssetManager::ItemType = TEXT("Item");

UCAssetManager& UCAssetManager::Get()
{
	UCAssetManager* This = Cast<UCAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		LOG_WARNING("Invalid AssetManager in DefaultEngine.ini, must be CAssetManager!");
		return *NewObject<UCAssetManager>(); //Never calls this.
	}
}

void UCAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//UAbilitySystemGlobals::Get().InitGlobalData();
}


UCData* UCAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{	
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	//This does a synchronous load and may hitch.
	UCData* LoadedItem = Cast<UCData>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		LOG_WARNING("Failed to load item for identifier", *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}