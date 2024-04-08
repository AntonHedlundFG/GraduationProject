// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData/CData.h"
#include "CData_Achievement.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCData_Achievement : public UCData
{
	GENERATED_BODY()

public:
	
	UCData_Achievement()
	{
		ItemType = UCAssetManager::AchievementItemType;
	}
	
};
