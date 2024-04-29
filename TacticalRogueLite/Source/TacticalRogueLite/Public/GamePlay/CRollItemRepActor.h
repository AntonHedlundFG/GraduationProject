// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CRollItemRepActor.generated.h"

//Open/Close ItemSelect widget
//Push info to widget from: SubSystem to: Widget (Replicate)

//ReplicateInfo(<TArray> Items)
//



UCLASS()
class TACTICALROGUELITE_API ACRollItemRepActor : public AInfo
{
	GENERATED_BODY()

	void ReplicateInfo();
};
