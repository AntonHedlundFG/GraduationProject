// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/CItemData.h"


FPrimaryAssetId UCItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
