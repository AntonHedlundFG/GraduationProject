#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRogueLiteRandomComponent.generated.h"

UCLASS()
class UCRogueLiteRandomComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCRogueLiteRandomComponent();
	
protected:
	virtual void BeginPlay() override;
	
public:
	// Initializes the random stream with a specific seed.
	void Init(int32 inSeed);

	// Generates a random value within a specified range, optionally without advancing the state.
	UFUNCTION(BlueprintCallable)
	int32 GetRandRange(int32 inMin, int32 inMax, bool bKeepState = true);

	// Overloaded method for generating a random value up to a max value, with optional state advancement.
	int32 GetRandRange(int32 Max, bool bKeepState = true);

	// Generates a completely random value, optionally without advancing the state.
	int32 GetRandRange(bool bKeepState = true);

	// Rolls back the random stream state by a specified number of ticks.
	UFUNCTION(BlueprintCallable)
	int32 RollBackRandom(int32 inTicks = 1);

	// Rolls back the random stream to the last saved state.
	UFUNCTION(BlueprintCallable)
	void RollBackToSave();

	// Saves the current state of the random stream.
	UFUNCTION(BlueprintCallable)
	void SaveState();

	// Resets the random stream to its initial seed, effectively restarting its state.
	UFUNCTION(BlueprintCallable)
	void ResetToInitialSeed();

	// Helper method to ensure seed values are within an acceptable range
	int32 ValidateSeed(int32 Seed);

	// Peek ahead
	UFUNCTION(BlueprintCallable)
	int32 PeekAhead(int32 inMin, int32 inMax, int32 inTicksAhead = 1) const;

	// Peeks ahead multiple times
	UFUNCTION(BlueprintCallable)
	TArray<int32> PeekAheadArray(const TArray<int32>& inMins, const TArray<int32>& inMaxs) const;

private:
	UPROPERTY(Replicated)
	FRandomStream RandomStream;

	UPROPERTY(Replicated)
	int64 Ticks;

	UPROPERTY(Replicated)
	int32 TicksSinceSave;

	UPROPERTY(Replicated)
	int32 TicksAtSave;

	UPROPERTY(Replicated)
	int32 StartSeed;

	UPROPERTY(Replicated)
	int32 SavedStateSeed;

	// Helper method to generate a debug message, compiled out in non-debug builds.
	void DebugMessage(const FString& Message) const;
	
protected:
	// Sets up property replication for the component.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

