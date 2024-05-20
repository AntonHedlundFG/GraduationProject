// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "CArmorActionVisualization.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCArmorActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()

protected:
	
	virtual void Enter() override;
	virtual bool Tick(float DeltaTime) override;

	UPROPERTY()
	class UCAction_RollItem* ActionClass;

};
