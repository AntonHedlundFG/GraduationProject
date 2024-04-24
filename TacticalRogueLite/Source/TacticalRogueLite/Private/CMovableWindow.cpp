// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CMovableWindow.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UCMovableWindow::ClearMoveTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(moveTimerHandle);
}

FVector2D UCMovableWindow::GetMousePosition()
{
	return UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
}
UCanvasPanelSlot* UCMovableWindow::GetCanvasPanelSlot()
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(CanvasPanelWrapper);
}
FVector2D UCMovableWindow::ClampWindowSize(FVector2D vector)
{
	float x = FMath::Clamp(vector.X,WindowMinSize.X,WindowMaxSize.X);
	float y = FMath::Clamp(vector.Y,WindowMinSize.Y,WindowMaxSize.Y);
	return FVector2D(x,y);
}
void UCMovableWindow::SetUp()
{
	HeaderButton->OnPressed.AddDynamic(this,&UCMovableWindow::OnHeaderButtonPressed);
	HeaderButton->OnReleased.AddDynamic(this,&UCMovableWindow::ClearMoveTimer);
	CloseButton->OnReleased.AddDynamic(this,&UCMovableWindow::Close);
	WindowMaxSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
}

void UCMovableWindow::UpdatePosition()
{
	FVector2D mousePos = GetMousePosition();
	UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanelSlot();
	FVector2D newPosition = mousePos - ClickDifference_Movable;
	float x = FMath::Clamp(newPosition.X, 0,WindowMaxSize.X);
	float y = FMath::Clamp(newPosition.Y, 0,WindowMaxSize.Y);
	CanvasPanelSlot->SetPosition(FVector2D(x,y));
}
void UCMovableWindow::StartMoveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(moveTimerHandle,this,&UCMovableWindow::UpdatePosition,0.01f,true);
}
void UCMovableWindow::OnHeaderButtonPressed()
{
	UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanelSlot();
	ClickDifference_Movable = GetMousePosition() - CanvasPanelSlot->GetPosition() ;
	StartMoveTimer();
}

void UCMovableWindow::NativeConstruct()
{
	Super::NativeConstruct();
	SetUp();
}

void UCMovableWindow::Close()
{
	bIsOpen = false;
	if(CloseSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),CloseSound);
	}
	MainCanvas->SetVisibility(ESlateVisibility::Collapsed);
	OnWindowClose.Broadcast();
}

void UCMovableWindow::Open()
{
	bIsOpen = true;
	if(OpenSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),OpenSound);
	}
	MainCanvas->SetVisibility(ESlateVisibility::Visible);
	OnWindowOpen.Broadcast();
}
