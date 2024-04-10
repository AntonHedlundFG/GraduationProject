// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "AnimatingVerticalBox.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UAnimatingVerticalBox : public UVerticalBox
{
	GENERATED_BODY()
public:
	void SwitchSlots(int a, int b);
};
