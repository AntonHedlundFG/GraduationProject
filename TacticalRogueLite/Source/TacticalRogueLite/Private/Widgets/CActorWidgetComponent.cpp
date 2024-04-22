// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CActorWidgetComponent.h"
#include "Widgets/CActorWidget.h"


UCActorWidgetComponent::UCActorWidgetComponent()
{
	//Set common defaults when using widgets on Actors.
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void UCActorWidgetComponent::InitWidget()
{
	// Base implementation creates the 'Widget' instance
	Super::InitWidget();

	
	if (GetWidget())
	{
#if !UE_BUILD_SHIPPING
		if (!GetWidget()->IsA(UCActorWidget::StaticClass()))
		{
			// Suggest deriving from actor widget (not strictly required, but you lose the benefit of auto-assigning the Actor this way)
			//UE_LOG(TEXT("WidgetClass for %s does not derive from SActorWidget"), *GetNameSafe(GetOwner());
		}
#endif
		UCActorWidget* WidgetInst = Cast<UCActorWidget>(GetWidget());
		if (WidgetInst)
		{
			// Assign the owner, now we have easy access in the widget itself
			WidgetInst->SetOwningActor(GetOwner());
		}
	}
}
