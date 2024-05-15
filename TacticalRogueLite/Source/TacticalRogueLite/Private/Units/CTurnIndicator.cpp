// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/CTurnIndicator.h"
#include "GridContent/CUnit.h"
#include "CGameState.h"
#include "PaperSpriteComponent.h"

// Sets default values
ACTurnIndicator::ACTurnIndicator()
{
	bReplicates = false;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACTurnIndicator::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = GetWorld()->GetGameState<ACGameState>();
	if (!IsValid(GameStateRef))
	{
		Destroy();
	}
}

// Called every frame
void ACTurnIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(GameStateRef)) return;
	
	ACUnit* CurrentUnit = GameStateRef->GetCurrentUnit();
	if (!IsValid(CurrentUnit)) return;

	//A new turn has started
	if (FollowingUnit != CurrentUnit)
	{
		SetUnitShaderActive(FollowingUnit, false);
		FollowingUnit = CurrentUnit;
		ResetAnimation();
		bEnableAnimation = FollowingUnit->IsControlledLocally(); //Only animate during my turn
		SetUnitShaderActive(FollowingUnit, bEnableAnimation);
		SetActorHiddenInGame(!bVisibleWhileNotYourTurn && !bEnableAnimation);
	}

	//Only update location if visible
	if (bEnableAnimation || bVisibleWhileNotYourTurn)
		SetActorLocation(FollowingUnit->GetActorLocation() + HeightOffset + TickAnimation(DeltaTime));
}

FVector ACTurnIndicator::TickAnimation(float DeltaTime)
{
	if (!bEnableAnimation) return FVector::ZeroVector; //No animation

	//If DeltaTime is greater than the rest of the animations duration, keep track of the remainder.
	const float ActualDeltaTime =
		bAnimatingForward ?
		FMath::Min(DeltaTime, AnimationDuration - CurrentAnimationTime) :
		FMath::Min(DeltaTime, CurrentAnimationTime);
	const float TimeRemainder = DeltaTime - ActualDeltaTime;

	//Invert time ticking if we're animating backwards.
	CurrentAnimationTime += bAnimatingForward ? ActualDeltaTime : -ActualDeltaTime;
	if (TimeRemainder > 0.0f)
	{
		CurrentAnimationTime += bAnimatingForward ? -TimeRemainder : TimeRemainder;
		bAnimatingForward = !bAnimatingForward;
	}

	return FMath::Lerp(FVector::ZeroVector, AnimationOffset, CurrentAnimationTime / AnimationDuration);
}

void ACTurnIndicator::ResetAnimation()
{
	CurrentAnimationTime = 0.0f;
}

void ACTurnIndicator::SetUnitShaderActive(ACUnit* InUnit, bool bInIsActive)
{
	if (!InUnit) return;
	UPaperSpriteComponent* Sprite = InUnit->GetComponentByClass<UPaperSpriteComponent>();
	if (Sprite)
		Sprite->SetScalarParameterValueOnMaterials(FName("bIsCurrentUnit"), bInIsActive ? 1.0f : 0.0f);
}
