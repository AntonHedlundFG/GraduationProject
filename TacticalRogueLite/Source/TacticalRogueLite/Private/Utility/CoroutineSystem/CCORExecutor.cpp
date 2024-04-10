// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CoroutineSystem/CCORExecutor.h"

#pragma region Executables

unsigned long long FExecutable::_nextId = 0;

FExecutable::FExecutable(): _id(_nextId++)
{
}

FExecutable::~FExecutable()
{
}

FExecutableHandle FExecutable::GenerateHandle()
{
	FExecutableHandle NewHandle; 
	NewHandle.ID = _id;
	return NewHandle;
}

unsigned long long FExecutable::GetID()
{
	return _id;
}

bool FExecutable::operator==(const FExecutable& obj) const
{
	return _id == obj._id;
}
#pragma endregion

#pragma region ExecutableOwner
void FExecutableContainer::Tick(float DeltaTime)
{
	if(Executables.Num() == 0 || bIsPaused)
	{
		return;
	}

	if(!Executables[0]->bHasStarted)
	{
		Executables[0]->OnStart();
		Executables[0]->bHasStarted = true;
	}
	bool bDoneExecuting = Executables[0]->Execute(DeltaTime);

	if(bDoneExecuting)
	{
		Executables[0]->OnEnd();
		delete Executables[0];
		Executables.RemoveAt(0);
	}
}

FExecutableContainer::FExecutableContainer()
{
	
}

FExecutableContainer::~FExecutableContainer()
{
	for (const auto Executable : Executables)
	{
		delete Executable;
	}
}
#pragma endregion 

FExecutableContainer* UCCORExecutor::GetExecutableContainer(UObject* Owner)
{
	if(!ExecutableContainers.Contains(Owner))
	{
		FExecutableContainer* ExecutableContainer =  new FExecutableContainer();
		ExecutableContainers.Add(Owner,ExecutableContainer);
		return  ExecutableContainer;
	}
	return ExecutableContainers[Owner];
}

void UCCORExecutor::TickGlobal(float DeltaTime)
{
	if(GlobalExecutables.Num() == 0)
	{
		return;
	}
	for (auto Element : GlobalExecutables)
	{
		FExecutable* Executable = Element.Value;
		if(!Executable->bPaused)
		{
			bool bDoneExecuting = Executable->Execute(DeltaTime);
			if(bDoneExecuting)
			{
				Executable->OnEnd();
				QueuedForDeletion.Add(Element.Key);
			}
		}
	}
	for (auto Key : QueuedForDeletion)
	{
		FExecutable* Executable = GlobalExecutables[Key];
		delete Executable;
		GlobalExecutables.Remove(Key);
	}
	QueuedForDeletion.Empty();
}

void UCCORExecutor::RemoveGlobalExecutable(FExecutableHandle Handle)
{
	FExecutable* Executable = GlobalExecutables[Handle.ID];
	GlobalExecutables.Remove(Handle.ID);
	if(Executable != nullptr)
	{
		delete Executable;
	}
}

FExecutableHandle UCCORExecutor::RunGlobalExecutable(FExecutable* Executable)
{
	Executable->OnStart();
	const FExecutableHandle Handle = Executable->GenerateHandle();
	GlobalExecutables.Add(Handle.ID,Executable);
	return Handle;
}

void UCCORExecutor::StartGlobalExecutable(FExecutableHandle Handle)
{
	if(GlobalExecutables.Contains(Handle.ID))
	{
		GlobalExecutables[Handle.ID]->bPaused = false;
	}
}

void UCCORExecutor::PauseGlobalExecutable(FExecutableHandle Handle)
{
	if(GlobalExecutables.Contains(Handle.ID))
	{
		GlobalExecutables[Handle.ID]->bPaused = true;
	}
}

FExecutableHandle UCCORExecutor::AddExecutable(UObject* Owner, FExecutable* Executable)
{
	FExecutableContainer* Container = GetExecutableContainer(Owner);
	Container->Executables.Add(Executable);
	return Executable->GenerateHandle();
}

void UCCORExecutor::RemoveExecutable(UObject* Owner, FExecutableHandle Handle)
{
	if(ExecutableContainers.Contains(Owner))
	{
		FExecutableContainer* Container = ExecutableContainers[Owner];
		for (auto Element : Container->Executables)
		{
			if(Element->GetID() == Handle.ID)
			{
				Container->Executables.Remove(Element);
				delete Element;
				return;
			}
		}
	}
}

void UCCORExecutor::Pause(UObject* Owner)
{
	if(ExecutableContainers.Contains(Owner))
	{
		ExecutableContainers[Owner]->bIsPaused = true;
	}
}
void UCCORExecutor::Start(UObject* Owner)
{
	if(ExecutableContainers.Contains(Owner))
	{
		ExecutableContainers[Owner]->bIsPaused = false;
	}
}

void UCCORExecutor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickGlobal(DeltaTime);
	for (auto Element : ExecutableContainers)
	{
		Element.Value->Tick(DeltaTime);
	}
}

UCCORExecutor::~UCCORExecutor()
{
	for (auto Element : ExecutableContainers)
	{
		delete Element.Value;
	}
	for (auto Element : GlobalExecutables)
	{
		delete Element.Value;
	}
}
