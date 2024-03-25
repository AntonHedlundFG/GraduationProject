// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class TACTICALROGUELITEEDITOR_API ConnectorVisualizer : public  FComponentVisualizer
{
private:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
public:
	ConnectorVisualizer();
	~ConnectorVisualizer();
};
