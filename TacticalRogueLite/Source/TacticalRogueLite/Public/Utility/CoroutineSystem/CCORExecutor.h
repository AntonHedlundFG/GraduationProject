#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Utility/Logging/CLogManager.h"
#include "CCORExecutor.generated.h"

UENUM(BlueprintType)
enum class EExecutableState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	Running UMETA(DisplayName = "Running"),
	Paused UMETA(DisplayName = "Paused"),
	Completed UMETA(DisplayName = "Completed")
};

struct FExecutableHandle 
{
	size_t ID;
};

UCLASS()
class TACTICALROGUELITE_API UExecutable : public UObject
{
	GENERATED_BODY()

public:
	UExecutable() : ID(NextId++) {}
	virtual ~UExecutable() override = default;

	virtual void Start() { State = EExecutableState::Running; }
	virtual void End() { State = EExecutableState::Completed; }
	void Initialize()
	{
		LOG_WARNING("Default implementation, should be overriden in derived class.");
	}
	virtual bool Execute(float DeltaTime)
	{
		LOG_WARNING("Default implementation, should be overriden in derived class.");
		return true;
	}

	void Pause() { State = EExecutableState::Paused; }
	void Unpause() { State = EExecutableState::Running; }

	bool HasStarted() const { return State != EExecutableState::NotStarted; }
	bool IsRunning() const { return State == EExecutableState::Running; }
	bool IsPaused() const { return State == EExecutableState::Paused; }
	
	FExecutableHandle GenerateHandle() const { return { ID }; }
	size_t GetID() const { return ID; }
	bool operator==(const UExecutable* obj) const { return ID == obj->ID; }

protected:
	float Timer = 0;
	EExecutableState State = EExecutableState::NotStarted;
	
private:
	size_t ID;
	static size_t NextId;
	
};

USTRUCT()
struct FExecutableContainer
{
	GENERATED_BODY()
	
	FExecutableContainer() {}
	~FExecutableContainer() {}
	
	void Tick(float DeltaTime);
	TArray<TObjectPtr<UExecutable>>& GetExecutables() { return Executables; }
	void Pause() { State = EExecutableState::Paused; }
	void Unpause() { State = EExecutableState::Running; }
	bool IsPaused() const { return State == EExecutableState::Paused; }
	
private:
	EExecutableState State = EExecutableState::Running;
	UPROPERTY()
	TArray<TObjectPtr<UExecutable>> Executables;
};

UCLASS()
class TACTICALROGUELITE_API UCCORExecutor : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	void RemoveGlobalExecutable( const FExecutableHandle& Handle);
	FExecutableHandle RunGlobalExecutable(UExecutable* Executable);
	void StartGlobalExecutable( const FExecutableHandle& Handle);
	void PauseGlobalExecutable( const FExecutableHandle& Handle);
	FExecutableHandle AddExecutable(UObject* Owner, UExecutable* Executable);
	void RemoveExecutable(const UObject* Owner, const FExecutableHandle& Handle);
	void Pause(const UObject* Owner);
	void Start(const UObject* Owner);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
	void TickGlobal(float DeltaTime);
		
private:

	UPROPERTY()
	TMap<UObject*, FExecutableContainer> ExecutableContainers;
	TMap<size_t, TObjectPtr<UExecutable>> GlobalExecutables;
	TArray<size_t> QueuedForDeletion; 
	FExecutableContainer& GetExecutableContainer(UObject* Owner);
};
