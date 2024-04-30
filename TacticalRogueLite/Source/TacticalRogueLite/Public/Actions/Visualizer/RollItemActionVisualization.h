// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "RollItemActionVisualization.generated.h"

class UCActorWidget;

UCLASS()
class TACTICALROGUELITE_API URollItemActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()
protected:

	UCActorWidget* Widget;
	
	virtual bool CanVisualizeAction_Implementation(UCAction* Action) override;
	virtual void Enter_Implementation() override;
	virtual bool Tick_Implementation(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Associated Action") //?
	class UCAction_RollItem* ActionClass;
	
};
