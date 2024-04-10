// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CCORExecutor.generated.h"

/**
 * 
 */
struct FExecutableHandle
{
	unsigned long long ID;
};
USTRUCT()
struct FExecutable
{
private:
	unsigned long long _id;
	static unsigned long long _nextId;

public:
	GENERATED_BODY()
	float Timer = 0;
	int State = 0;
	bool bHasStarted = false;
	bool bPaused = false;
	FExecutable();
	/*!
	 *Called once before the Execute function is run 
	 */
	virtual void OnStart(){}
	/*!
	 *Called after the Executable is done.
	 */
	virtual void OnEnd(){}
	/*!
	 *Called as long as Execute is returning false.
	 *@return Return false as long as you want to run the executable, return true for when the executable is done.
	 */
	virtual bool Execute(float DeltaTime){return true;}
	virtual ~FExecutable();
	FExecutableHandle GenerateHandle();
	unsigned long long GetID();
	bool operator==(const FExecutable& obj) const;
};
struct FExecutableContainer
{
	bool bIsPaused = false;
	TArray<FExecutable*> Executables;
	void Tick(float DeltaTime);
	FExecutableContainer();
	~FExecutableContainer();
};

UCLASS()
class TACTICALROGUELITE_API UCCORExecutor : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	TMap<UObject*,FExecutableContainer*> ExecutableContainers;
	TMap<unsigned long long,FExecutable*> GlobalExecutables;
	TArray<unsigned long long> QueuedForDeletion; 
private:
	FExecutableContainer* GetExecutableContainer(UObject* Owner);
	void TickGlobal(float DeltaTime);
public:
	
	/*
	 *Removes an Executable running in the global executable list, will also call OnEnd() and delete ít
	 */
	void RemoveGlobalExecutable(FExecutableHandle Handle);
	/*!
	 * Runs the Executable until finished.
	 */
	FExecutableHandle RunGlobalExecutable(FExecutable* Executable);
	/*!
	 * Resumes an Executable running in the global executable list
	 */

	void StartGlobalExecutable(FExecutableHandle Handle);
	/*!
	* Pauses an Executable running in the global executable list 
	*/
	void PauseGlobalExecutable(FExecutableHandle Handle);
	/*!
	 * Adds an Executable to the back of the executable queue.
	 */
	FExecutableHandle AddExecutable(UObject* Owner,FExecutable* Executable);
	/*!
	 * Removes Executable from queue. If it is running it will call the OnEnd Function in the executable and be deleted.
	 */
	void RemoveExecutable(UObject* Owner,FExecutableHandle Handle);
	/*!
	 *	Pauses the current running Executable on owner.
	 */
	void Pause(UObject* Owner);
	/*!
	 *	Starts the current running executable on owner.
	 */
	void Start(UObject* Owner);
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}
	virtual ~UCCORExecutor();
	
};
