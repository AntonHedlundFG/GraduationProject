// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Attributes/Utilities/CAttribute.h"
#include "Utility/TurnTimer/CTurnTimerSubsystem.h"
#include "CActionWithTimer.generated.h"


UCLASS()
class TACTICALROGUELITE_API UCActionWithTimer : public UCAction
{
	GENERATED_BODY()

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void UndoAction_Implementation(AActor* Instigator) override;

	// List of modifiers to apply to attributes when the action is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeModification> Modifiers;

	

	//OnTimerFinishes_Implementation can be overridden to change the outcome
	//of the timer finishing.
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnTimerFinishes(ACUnit* inAffectedUnit);

	//FOR ANGELSCRIPT: Call this from your StartAction implementation to start 
	//the timer. Make sure you set AffectedUnit first, since this is done in
	//the C++ default implementation which does not get called.
	UFUNCTION(BlueprintCallable, Category = "Action")
	void BindTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int NumberOfTurns = 1;

	//The unit which the timer will depend on. If nothing is assigned manually
	//it will attempt to use the Instigator if it's a ACUnit.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Action")
	class ACUnit* AffectedUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bShouldLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EProgressTurnMethod ProgressTurnMethod = EProgressTurnMethod::EPTM_StartOfTurn;

	int32 GetLevel() const { return Level; }

protected:

	int32 Level = 1; //?

	FOnTurnTimerExecute TimerDelegate;
	FTurnTimerHandle TimerHandle;

};
