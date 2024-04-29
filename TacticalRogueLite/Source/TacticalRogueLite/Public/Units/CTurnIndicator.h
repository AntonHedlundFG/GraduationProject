// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTurnIndicator.generated.h"

/// <summary>
/// A visual indicator for whose turn it is. 
/// The GameState stores a TSubclassOf this class and spawns it in BeginPlay.
/// It hovers over the unit whose turn it is, and animates up and down if it's your turn.
/// </summary>
UCLASS()
class TACTICALROGUELITE_API ACTurnIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ACTurnIndicator();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	//Base offset from the unit whose turn it is
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn Indicator")
	FVector HeightOffset = FVector(0, 0, 200);

	//Length of animation in one direction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn Indicator")
	float AnimationDuration = 1.0f;

	//Maximum additional offset when animation is at its peak
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn Indicator")
	FVector AnimationOffset = FVector(0, 0, 50);

	//If disabled, the indicator is hidden when it's another players turn (or AI)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn Indicator")
	bool bVisibleWhileNotYourTurn = false;

	UPROPERTY()
	TObjectPtr<class ACGameState> GameStateRef;

	UPROPERTY()
	class ACUnit* FollowingUnit = nullptr;

	//Ticks the animation forward and returns the current additional offset
	FVector TickAnimation(float DeltaTime);
	void ResetAnimation();
	float CurrentAnimationTime = 0.0f;
	bool bAnimatingForward = true;
	bool bEnableAnimation = false;
	
	void SetUnitShaderActive(ACUnit* InUnit, bool bInIsActive);

};
