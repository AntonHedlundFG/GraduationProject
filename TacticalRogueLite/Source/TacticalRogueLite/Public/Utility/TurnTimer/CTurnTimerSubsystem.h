// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CTurnTimerSubsystem.generated.h"

class ACUnit;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTurnTimerExecute, ACUnit*, AffectedUnit);

UENUM(BlueprintType)
enum class EProgressTurnMethod : uint8
{
	EPTM_None,
	EPTM_StartOfTurn,
	EPTM_EndOfTurn,
	EPTM_Max
};

// A return type created by SetTimer(), used to ClearTimer()
USTRUCT(BlueprintType)
struct FTurnTimerHandle
{
	GENERATED_BODY()

	friend class UCTurnTimerSubsystem;

public:
	FTurnTimerHandle() {}
	FTurnTimerHandle(uint32 ID)
	{
		TimerID = ID;
	}

protected:

	uint32 TimerID = 0;

};

//Internal struct keeping track of an individual timer
USTRUCT()
struct FTurnTimer
{
	GENERATED_BODY()

public:

	uint8 NumberOfTurns = 0;
	uint8 TurnsRemaining = 0;
	bool bLooping = false;
	EProgressTurnMethod ProgressTurnMethod = EProgressTurnMethod::EPTM_None;
	ACUnit* AffectedUnit;
	FOnTurnTimerExecute BoundDelegate;

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
	/// Pass in a bound FOnTurnTimerExecute delegate, which will execute when the
	/// timer activates.
	/// </summary>
	/// <param name="InNumberOfTurns">How many turns until activation</param>
	/// <param name="InAffectedUnit">The unit whose turn is counted</param>
	/// <param name="InbLoop"> Should the timer reset and start again on activation? </param>
	/// <param name="InProgressTurnMethod"> Should the timer activate at the end or start of the affected units turn?</param>
	/// <returns>A handle which can be used to identify and clear the set timer.</returns>
	UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	FTurnTimerHandle SetTimer(const int InNumberOfTurns, 
				ACUnit* InAffectedUnit,
				const bool InbLoop, 
				const EProgressTurnMethod InProgressTurnMethod, 
				FOnTurnTimerExecute InDelegate);

	//Invalidates the handle and removes the related timer.
	UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	void ClearTimer(FTurnTimerHandle InHandle);

	UFUNCTION(BlueprintCallable, Category = "Turn Timer")
	bool IsTimerActive(FTurnTimerHandle InHandle);

protected:

	// This is called by the GameMode to move the timer forward. 
	// Do not call this manually
	void NextTurn(const ACUnit* PreviousUnit, const ACUnit* NextUnit);

	TMap<uint32, FTurnTimer> ActiveTurnTimers;

	//Used internally to generate ID for new timers.
	uint32 LastTimerID = 0;
	
};
