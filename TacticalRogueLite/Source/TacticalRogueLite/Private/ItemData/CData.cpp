// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/CData.h"

FText UCData::GetDescription_Implementation() const
{
	return ItemDescription;
}

FText UCData::GetTitle_Implementation() const
{
	return ItemName;
}

FText UCData::GetItemName_Implementation() const
{
	return ItemName;
}
