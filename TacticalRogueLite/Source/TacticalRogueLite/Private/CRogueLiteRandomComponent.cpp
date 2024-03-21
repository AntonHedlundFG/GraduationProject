#include "CRogueLiteRandomComponent.h"

#include "Net/UnrealNetwork.h"

UCRogueLiteRandomComponent::UCRogueLiteRandomComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCRogueLiteRandomComponent::BeginPlay()
{
	Super::BeginPlay();
	Init(0);
}


void UCRogueLiteRandomComponent::Init(int32 inSeed)
{
	Ticks = 0;
	TicksSinceSave = 0;
	TicksAtSave = 0;
	StartSeed = ValidateSeed(inSeed); // Validate seed before use
	RandomStream.Initialize(StartSeed);
	SaveState();
}

int32 UCRogueLiteRandomComponent::GetRandRange(int32 inMin, int32 inMax, bool bKeepState)
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

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Value: %d, Ticks: %lld, Ticks since save: %d"), Value, Ticks, TicksSinceSave));

	
	return Value;
}

int32 UCRogueLiteRandomComponent::GetRandRange(int32 Max, bool bKeepState)
{
	if(Max < 0)
		return GetRandRange(bKeepState);
	return GetRandRange(0, Max, bKeepState);
}

int32 UCRogueLiteRandomComponent::GetRandRange(bool bKeepState)
{
	return GetRandRange( 0,INT32_MAX - 1, bKeepState);
}

int32 UCRogueLiteRandomComponent::RollBackRandom(int32 inTicks)
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

	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Red,
	FString::Printf(TEXT("Value: %d, Ticks: %lld, Ticks since save: %d"), Value, Ticks, TicksSinceSave));

	
	return Value;
}

void UCRogueLiteRandomComponent::RollBackToSave()
{
	RandomStream.Initialize(SavedStateSeed);
	TicksSinceSave = 0;
	Ticks = TicksAtSave;
}

void UCRogueLiteRandomComponent::SaveState()
{
	SavedStateSeed = RandomStream.GetCurrentSeed();
	TicksSinceSave = 0;
	TicksAtSave = Ticks;
}

void UCRogueLiteRandomComponent::ResetToInitialSeed()
{
	Ticks = 0;
	TicksSinceSave = 0;
	RandomStream.Initialize(StartSeed);
	SaveState();
}

int32 UCRogueLiteRandomComponent::ValidateSeed(int32 Seed)
{
	return FMath::Clamp(Seed, 0, INT32_MAX - 1);
}

int32 UCRogueLiteRandomComponent::PeekAhead(int32 inMin, int32 inMax, int32 inTicksAhead) const
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

TArray<int32> UCRogueLiteRandomComponent::PeekAheadArray(const TArray<int32>& inMins, const TArray<int32>& inMaxs) const
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


void UCRogueLiteRandomComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCRogueLiteRandomComponent, RandomStream);
	DOREPLIFETIME(UCRogueLiteRandomComponent, Ticks);
	DOREPLIFETIME(UCRogueLiteRandomComponent, StartSeed);
	DOREPLIFETIME(UCRogueLiteRandomComponent, SavedStateSeed);
	DOREPLIFETIME(UCRogueLiteRandomComponent, TicksSinceSave);
	DOREPLIFETIME(UCRogueLiteRandomComponent, TicksAtSave);
}
