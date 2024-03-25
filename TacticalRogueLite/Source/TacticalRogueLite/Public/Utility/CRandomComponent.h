#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveGame/CSavable.h"
#include "CRandomComponent.generated.h"

UCLASS()
class UCRandomComponent : public UActorComponent, public ICSavable
{
	GENERATED_BODY()
public:
	UCRandomComponent();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// Initializes the random stream with a specific seed.
	void InitializeFromStart(int32 inStartSeed);

	// Initializes the random stream with a specific seed and state.
	void InitializeFromSave(int32 inStartSeed, int32 inCurrentStateSeed, int64 inTicks, int32 inTicksSinceSave,
	                        int32 inTicksAtSave);

	// Generates a random value within a specified range, optionally without advancing the state.
	UFUNCTION(BlueprintCallable)
	int32 GetRandRange(int32 inMin, int32 inMax, bool bKeepState = true);

	// Generates a random value within 0 to inMax, optionally without advancing the state
	int32 GetRandRange(int32 inMax, bool bKeepState = true);

	// Generates a random value within 0 and max int32, optionally without advancing the state.
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

	// Peek ahead a specified number of ticks
	UFUNCTION(BlueprintCallable)
	int32 PeekAhead(int32 inMin, int32 inMax, int32 inTicksAhead = 1) const;

	// Peeks ahead multiple times
	UFUNCTION(BlueprintCallable)
	TArray<int32> PeekAheadArray(const TArray<int32>& inMins, const TArray<int32>& inMaxs) const;

	// OnSave implementation
	virtual void OnSave() override;

	// OnLoad implementation
	virtual void OnLoad() override;
<<<<<<< HEAD

	// RegisterToSaveManager implementation
	virtual void RegisterToSaveManager() override;

	// UnregisterFromSaveManager implementation
	virtual void UnregisterFromSaveManager() override;
=======
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
	
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
	int32 CurrentStateSeed;

protected:
	// Sets up property replication for the component.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

