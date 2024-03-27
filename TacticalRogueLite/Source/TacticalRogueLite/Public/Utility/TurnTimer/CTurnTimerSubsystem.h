// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CTurnTimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnTimerExecute, ACUnit*, AffectedUnit);

class ACUnit;

UENUM(BlueprintType)
enum EProgressTurnMethod : uint8
{
	EPTM_None,
	EPTM_StartOfTurn,
	EPTM_EndOfTurn,
	EPTM_Max
};

USTRUCT(BlueprintType)
struct FTurnTimerHandle
{
	GENERATED_BODY()

	friend class UCTurnTimerSubsystem;

public:

	UPROPERTY(BlueprintAssignable, Category = "Turn Timer")
	FOnTurnTimerExecute OnTurnTimerExecute;

	bool IsValid() { return bIsValid; }

protected:

	uint32 TimerID = 0;

	uint8 NumberOfTurns = 0;
	uint8 TurnsRemaining = 0;
	bool bLooping = false;
	EProgressTurnMethod ProgressTurnMethod = EProgressTurnMethod::EPTM_None;
	ACUnit* AffectedUnit;

	UPROPERTY(BlueprintReadOnly, Category = "Turn Timer")
	bool bIsValid = false;
};

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCTurnTimerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	friend class ACGameMode;

public:

	/// <summary>
	/// Should work just like SetTimer in the usual FTimerManager, except
	/// instead of using floats for duration, we use integers to describe
	/// number of turns until activation.
	/// Subscribe to the OnTurnTimerExecute delegate in the FTurnTimerHandle
	/// to handle callbacks when the timer activates.
	/// </summary>
	/// <param name="InOutHandle"> Timer Handle, can be used to ClearTimer()</param>
	/// <param name="InNumberOfTurns">How many turns until activation</param>
	/// <param name="InAffectedUnit">The unit whose turn is counted</param>
	/// <param name="InbLoop"> Should the timer reset and start again on activation? </param>
	/// <param name="InProgressTurnMethod"> Should the timer activate at the end or start of the affected units turn?</param>
	UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	void SetTimer(FTurnTimerHandle& InOutHandle, 
				const int InNumberOfTurns, 
				ACUnit* InAffectedUnit,
				const bool InbLoop, 
				const EProgressTurnMethod InProgressTurnMethod);

	//Invalidates the handle and removes the related timer.
	UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	void ClearTimer(FTurnTimerHandle& InOutHandle);

protected:

	// This is called by the GameMode to move the timer forward. 
	// Do not call this manually
	void NextTurn(const ACUnit* PreviousUnit, const ACUnit* NextUnit);

	TMap<uint32, FTurnTimerHandle*> ActiveTurnTimers;

	uint32 LastTimerID = 0;
	
};
