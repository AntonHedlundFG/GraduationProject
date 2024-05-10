// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "Actions/CAction.h"
#include "CAoeActionVisualization.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAoeActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()
public:
	virtual bool Tick(float DeltaTime) override;
	virtual bool RevertTick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class UCAction>> AffectedActionTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> OnHitEffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> AreaEffectType;
};
