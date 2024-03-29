// Fill out your copyright notice in the Description page of Project Settings.


#include "CResizableWindow.h"
#include "Components/CanvasPanelSlot.h"

void UCResizableWindow::ClearResizeTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(resizeTimerHandle);
}



void UCResizableWindow::SetClickDifference()
{
	FVector2D mousePos = GetMousePosition();
	UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanelSlot();
	ClickDifference = mousePos - (CanvasPanelSlot->GetPosition() + CanvasPanelSlot->GetSize());
}

void UCResizableWindow::SetUp()
{
	Super::SetUp();
	
	DiagonalButton->OnPressed.AddDynamic(this,&UCResizableWindow::OnDiagonalPressed);
	HorizontalButton->OnPressed.AddDynamic(this,&UCResizableWindow::OnHorizontalPressed);
	VerticalButton->OnPressed.AddDynamic(this,&UCResizableWindow::OnVerticalPressed);
	
	DiagonalButton->OnReleased.AddDynamic(this,&UCResizableWindow::ClearResizeTimer);
	HorizontalButton->OnReleased.AddDynamic(this,&UCResizableWindow::ClearResizeTimer);
	VerticalButton->OnReleased.AddDynamic(this,&UCResizableWindow::ClearResizeTimer);
}

void UCResizableWindow::UpdateScale()
{
	FVector2D NewCanvasScale = GetNewCanvasScale();
	NewCanvasScale = ClampWindowSize(NewCanvasScale);
	UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanelSlot();
	FVector2D OldCanvasScale = CanvasPanelSlot->GetSize();
	FVector2D horizontalCanvasScale = FVector2D::Zero();
	FVector2D verticalCanvasScale = FVector2D::Zero();
	switch (ScalingType)
	{
		case EResizeType::Diagonal:
			CanvasPanelSlot->SetSize(NewCanvasScale);
			break;
		
		case EResizeType::Horizontal:
			horizontalCanvasScale = FVector2D(NewCanvasScale.X,OldCanvasScale.Y);
			CanvasPanelSlot->SetSize(horizontalCanvasScale);
			break;
		
		case EResizeType::Vertical:
			verticalCanvasScale = FVector2D(OldCanvasScale.X,NewCanvasScale.Y);
			CanvasPanelSlot->SetSize(verticalCanvasScale);
			break;
		case EResizeType::None: return;
	}
}

void UCResizableWindow::StartResizeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(resizeTimerHandle,this,&UCResizableWindow::UpdateScale,0.01f,true);
}



void UCResizableWindow::OnDiagonalPressed()
{
	ScalingType = EResizeType::Diagonal;
	SetClickDifference();
	StartResizeTimer();
}

void UCResizableWindow::OnHorizontalPressed()
{
	ScalingType = EResizeType::Horizontal;
	SetClickDifference();
	StartResizeTimer();
}

void UCResizableWindow::OnVerticalPressed()
{
	ScalingType = EResizeType::Vertical;
	SetClickDifference();
	StartResizeTimer();
}


FVector2D UCResizableWindow::GetNewCanvasScale()
{
	FVector2D mousePos = GetMousePosition();
	
	UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanelSlot();
	FVector2D ClickDelta = mousePos - ClickDifference;
	FVector2D NewSize = CanvasPanelSlot->GetPosition() + CanvasPanelSlot->GetSize();
	return ClickDelta - NewSize + CanvasPanelSlot->GetSize();
}

