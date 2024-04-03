// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CAbilityWidget.h"
#include "Attributes/CAttributeComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Utility/Logging/CLogManager.h"


void UCAbilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		LOG_WARNING("AttachedActor no longer valid, removing Ability Widget");
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);

	if (bIsOnScreen)
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
