// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
#include "ItemData/CItemData.h"
#include "CClassSpecifics.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCClassSpecifics : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UPaperSprite*> ClassSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ClassName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag ClassTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UCItemData*> Items;
};
