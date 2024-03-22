﻿#include "Utility/CRandomComponent.h"

#include "Net/UnrealNetwork.h"
#include "Utility/SaveGame/CSaveGame.h"
#include "Utility/SaveGame/CSaveGameManager.h"

UCRandomComponent::UCRandomComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCRandomComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterToSaveManager();
}

void UCRandomComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnregisterFromSaveManager();
}

void UCRandomComponent::InitializeFromStart(int32 inStartSeed)
{
	StartSeed = ValidateSeed(inStartSeed); 
	CurrentStateSeed = StartSeed;
	Ticks = 0;
	TicksSinceSave = 0;
	TicksAtSave = 0;

	RandomStream.Initialize(CurrentStateSeed);
	SaveState();
}

int32 UCRandomComponent::GetRandRange(int32 inMin, int32 inMax, bool bKeepState)
{
	if (inMax < inMin) { Swap(inMax, inMin); } // Correct inverted ranges.
	const int32 Range = (inMax - inMin) + 1;
	
	if (Range <= 0) { return inMin; } // If the range is invalid, return the min
	
	int32 Value;
	if(bKeepState)
	{
		// Temporarily create a copy to get value without advancing the stream
        const FRandomStream TempStream = RandomStream;
		Value = inMin + TempStream.RandHelper(Range);
	}
	else
	{
		// Get the value and advance the stream
		Value = inMin + RandomStream.RandHelper(Range);
		Ticks++;
		TicksSinceSave++;

	}
	
	return Value;
}

int32 UCRandomComponent::GetRandRange(int32 inMax, bool bKeepState)
{
	if(inMax < 0)
		return GetRandRange(bKeepState);
	return GetRandRange(0, inMax, bKeepState);
}

int32 UCRandomComponent::GetRandRange(bool bKeepState)
{
	return GetRandRange( 0,INT32_MAX - 1, bKeepState);
}

int32 UCRandomComponent::RollBackRandom(int32 inTicks)
{
	if (inTicks < 1)
		return -1;
	
	int32 TargetTicks = TicksSinceSave - inTicks;
	
	if(TargetTicks < 0)
		return -1;

	RollBackToSave();
	int32 Value = -1;
	for(int i = 0; i < TargetTicks; i++)
	{
		// Don't need the actual value, just need to advance the stream
		Value = RandomStream.RandHelper(1);
		Ticks++;
		TicksSinceSave++;
	}
	
	return Value;
}

void UCRandomComponent::RollBackToSave()
{
	RandomStream.Initialize(CurrentStateSeed);
	TicksSinceSave = 0;
	Ticks = TicksAtSave;
}

void UCRandomComponent::SaveState()
{
	CurrentStateSeed = RandomStream.GetCurrentSeed();
	TicksSinceSave = 0;
	TicksAtSave = Ticks;
}

void UCRandomComponent::ResetToInitialSeed()
{
	Ticks = 0;
	TicksSinceSave = 0;
	RandomStream.Initialize(StartSeed);
	SaveState();
}

int32 UCRandomComponent::ValidateSeed(int32 Seed)
{
	return FMath::Clamp(Seed, INT32_MIN, INT32_MAX);
}

int32 UCRandomComponent::PeekAhead(int32 inMin, int32 inMax, int32 inTicksAhead) const
{
	if (inMax < inMin) { Swap(inMax, inMin); } // Correct inverted ranges.
	const int32 Range = (inMax - inMin) + 1;
	
	if (Range <= 0) { return inMin; } // If the range is invalid, return the min

	// Make a copy of the current random stream
	const FRandomStream TempStream = RandomStream; 

	// Advance the copy stream by the specified number of ticks
	for (int i = 0; i < inTicksAhead - 1; i++)
	{
		TempStream.RandHelper(1); 
	}

	const int32 Value = TempStream.RandRange(inMin, inMax); 

	return Value; 
}

TArray<int32> UCRandomComponent::PeekAheadArray(const TArray<int32>& inMins, const TArray<int32>& inMaxs) const
{
	TArray<int32> Values;
	if (inMins.Num() != inMaxs.Num() || inMins.Num() <= 0) 
	{
		// Ensure valid input
		return Values;
	}

	const FRandomStream TempStream = RandomStream; // Make a copy of the current random stream

	for (int i = 0; i < inMins.Num(); i++)
	{
		int32 Min = FMath::Min(inMins[i], inMaxs[i]);
		const int32 Max = FMath::Max(inMins[i], inMaxs[i]);
		const int32 Range = (Max - Min) + 1;

		if (Range <= 0) 
		{
			// If the range is invalid, use the Min value
			Values.Add(Min);
		}
		else
		{
			// Generate and add the next random value within the specified range
			Values.Add(TempStream.RandRange(Min, Max));
		}
	}

	return Values;
}

void UCRandomComponent::OnSave()
{
	UCSaveGame* SaveGame = UCSaveGameManager::GetInstance()->GetSaveGameInstance();
	if (SaveGame)
	{
		SaveGame->SavedRandomStream = RandomStream;
		SaveGame->SavedStartSeed = StartSeed;
		SaveGame->SavedCurrentStateSeed = CurrentStateSeed;
		SaveGame->SavedTicks = Ticks;
		SaveGame->SavedTicksSinceSave = TicksSinceSave;
		SaveGame->SavedTicksAtSave = TicksAtSave;
	}
}

void UCRandomComponent::OnLoad()
{
	UCSaveGame* SaveGame = UCSaveGameManager::GetInstance()->GetSaveGameInstance();
	if (SaveGame)
	{
		RandomStream = SaveGame->SavedRandomStream;
		StartSeed = ValidateSeed(SaveGame->SavedStartSeed); 
		CurrentStateSeed = ValidateSeed(SaveGame->SavedCurrentStateSeed);
		Ticks = SaveGame->SavedTicks;
		TicksSinceSave = SaveGame->SavedTicksSinceSave;
		TicksAtSave = SaveGame->SavedTicksAtSave;
	}
}

void UCRandomComponent::RegisterToSaveManager()
{
	UCSaveGameManager::GetInstance()->RegisterSavable(this);
	OnLoad();
}

void UCRandomComponent::UnregisterFromSaveManager()
{
	UCSaveGameManager::GetInstance()->UnRegisterSavable(this);
}

void UCRandomComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCRandomComponent, RandomStream);
	DOREPLIFETIME(UCRandomComponent, Ticks);
	DOREPLIFETIME(UCRandomComponent, StartSeed);
	DOREPLIFETIME(UCRandomComponent, CurrentStateSeed);
	DOREPLIFETIME(UCRandomComponent, TicksSinceSave);
	DOREPLIFETIME(UCRandomComponent, TicksAtSave);
}
