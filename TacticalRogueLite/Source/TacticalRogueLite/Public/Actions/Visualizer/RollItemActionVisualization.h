// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/Visualizer/CActionVisualization.h"
#include "UI/CItemSelectionWindow.h"
#include "RollItemActionVisualization.generated.h"

class UCActorWidget;

UCLASS()
class TACTICALROGUELITE_API URollItemActionVisualization : public UCActionVisualization
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCItemSelectionWindow> WidgetClass;
	UPROPERTY()
	UCItemSelectionWindow* ItemSelectionWindow;
	
	virtual bool CanVisualizeAction(UCAction* Action) override;
	virtual void Enter() override;
	virtual bool Tick(float DeltaTime) override;

	UPROPERTY()
	class UCAction_RollItem* ActionClass;
	
	void OnItemSelectedCallback(UCItemData* SelectedItem);	
};
