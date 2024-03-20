// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CItem.h"

bool UCItem::IsValidTargetTile(ACUnit* inUnit, ACGridTile* inTargetTile)
{
	TArray<ACGridTile*> ValidTiles = GetValidTiles(inUnit);
	return ValidTiles.Contains(inTargetTile);
}

bool UCItem::IsValidTargetTileIndex(ACUnit* inUnit, const int inTargetTileIndex)
{
	ACGridTile* TargetTile = nullptr; //Should be Grid->GetTileOfIndex(inTargetTileIndex)
	return IsValidTargetTile(inUnit, TargetTile);
}