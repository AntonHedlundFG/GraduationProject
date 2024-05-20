// Fill out your copyright notice in the Description page of Project Settings.

#include "Actions/Visualizer/CArmorActionVisualization.h"

#include "Actions/CActionComponent.h"

void UCArmorActionVisualization::Enter()
{
	Super::Enter();
	
	//ActionClass = Cast<UCAction>(VisualizedAction);
	

	//UCActionComponent* ActionComponent = ActionClass->GetActionComp();
	// if(ActionComponent)
	// {
	// 	ACUnit* Unit = Cast<ACUnit>(ActionClass->GetActionComp()->GetOuter());
	// 	if (Unit)
	// 	{
	// 		
	// 	}
	// }
}

bool UCArmorActionVisualization::Tick(float DeltaTime)
{
	/*return Super::Tick(DeltaTime);

	if (!ActionClass->IsRunning())
	{
		if (ItemSelectionWindow)
		{
			ItemSelectionWindow->Close();
		}
		return true;
	}
*/
	return false;
}
