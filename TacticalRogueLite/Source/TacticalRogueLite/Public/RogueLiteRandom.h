#pragma once

class RogueLiteRandom
{	
public:

	RogueLiteRandom() : Ticks(0), StartSeed(0), SavedStateSeed(0), TicksSinceSave(0) {}
	~RogueLiteRandom() {}
	
	void Init(int32 inSeed);

	int32 GetRandRange(int32 inMin, int32 inMax, bool bKeepState = true);
	int32 GetRandRange(int32 Max, bool bKeepState = true);
	int32 GetRandRange(bool bKeepState = false);
	int32 RollBackRandom(int32 inTicks);
	void SaveState();

	// GetState
	// Reset
	// Get Current seed
	
private:
	FRandomStream RandomStream;
	int64 Ticks;
	int32 StartSeed;
	int32 SavedStateSeed;
	int32 TicksSinceSave;
	
};
