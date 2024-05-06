// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
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
};
