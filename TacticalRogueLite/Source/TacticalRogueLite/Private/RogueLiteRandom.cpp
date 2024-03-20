#include "RogueLiteRandom.h"

void RogueLiteRandom::Init(int32 inSeed)
{
	Ticks = 0;
	StartSeed = inSeed;
	SavedStateSeed = inSeed;
	RandomStream.Initialize(StartSeed);
}

int32 RogueLiteRandom::GetRandRange(int32 inMin, int32 inMax, bool bKeepState)
{
	const int32 Range = (inMax - inMin) + 1;
	int32 Value;

	if(bKeepState)
	{
		const FRandomStream TempStream = RandomStream;
		Value = inMin + TempStream.RandHelper(Range);
	}
	else
	{
		Value = inMin + RandomStream.RandHelper(Range);
	}
	Ticks++;
	TicksSinceSave++;
	return Value;
}

int32 RogueLiteRandom::GetRandRange(int32 Max, bool bKeepState)
{
	return GetRandRange(0, Max, bKeepState);
}

int32 RogueLiteRandom::GetRandRange(bool bKeepState)
{
	return GetRandRange( 0,INT32_MAX - 1, bKeepState);
}

int32 RogueLiteRandom::RollBackRandom(int32 inTicks)
{
	Ticks -= inTicks;

	RandomStream.Initialize(SavedStateSeed);
	int32 Value = -1;
	for(int i = 0; i < Ticks; i++)
	{
		// Don't need the actual value, just need to advance the stream
		Value = RandomStream.RandHelper(1);
	}
	return Value;
}

void RogueLiteRandom::SaveState()
{
	SavedStateSeed = RandomStream.GetCurrentSeed();
	TicksSinceSave = 0;
}