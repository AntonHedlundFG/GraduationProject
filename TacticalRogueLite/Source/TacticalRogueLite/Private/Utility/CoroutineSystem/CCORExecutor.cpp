#include "Utility/CoroutineSystem/CCORExecutor.h"

size_t UExecutable::NextId = 0;

void FExecutableContainer::Tick( float DeltaTime )
{
	if(Executables.Num() == 0 || IsPaused())	return;

	UExecutable* Executable = Executables[0];
	if(!Executable->HasStarted())
	{
		Executable->Start();
	}

	if(Executable->Execute(DeltaTime))
	{
		Executable->End();
		Executables.RemoveAt(0);
	}
}

FExecutableContainer& UCCORExecutor::GetExecutableContainer(UObject* Owner)
{
	if(!ExecutableContainers.Contains(Owner))
	{
		ExecutableContainers.Add(Owner, FExecutableContainer());
	}
	return ExecutableContainers[Owner];
}

void UCCORExecutor::TickGlobal(float DeltaTime)
{
	if (GlobalExecutables.Num() == 0) return;

	for (auto& Element : GlobalExecutables)
	{
		UExecutable* Executable = Element.Value;
		if (!Executable->IsPaused())
		{
			if (Executable->Execute(DeltaTime))
			{
				Executable->End();
				QueuedForDeletion.Add(Element.Key);
			}
		}
	}

	for (const auto& Key : QueuedForDeletion)
	{
		if (TObjectPtr<UExecutable>* ExecutablePtr = GlobalExecutables.Find(Key))
		{
			GlobalExecutables.Remove(Key);
		}
	}
	QueuedForDeletion.Empty();
}

void UCCORExecutor::RemoveGlobalExecutable(const FExecutableHandle& Handle)
{
	if (TObjectPtr<UExecutable>* ExecutablePtr = GlobalExecutables.Find(Handle.ID))
	{
		GlobalExecutables.Remove(Handle.ID); 
	}
}

FExecutableHandle UCCORExecutor::RunGlobalExecutable(UExecutable* Executable)
{
	if (!Executable) return FExecutableHandle{};
	
	Executable->Start();
	const FExecutableHandle Handle = Executable->GenerateHandle();
	GlobalExecutables.Add(Handle.ID, Executable);
	return Handle;
}

void UCCORExecutor::StartGlobalExecutable(const FExecutableHandle& Handle)
{
	if (TObjectPtr<UExecutable>* ExecutablePtr = GlobalExecutables.Find(Handle.ID))
	{
		(*ExecutablePtr)->Unpause();
	}
}

void UCCORExecutor::PauseGlobalExecutable(const FExecutableHandle& Handle)
{
	if (TObjectPtr<UExecutable>* ExecutablePtr = GlobalExecutables.Find(Handle.ID))
	{
		(*ExecutablePtr)->Pause();
	}
}

FExecutableHandle UCCORExecutor::AddExecutable(UObject* Owner, UExecutable* Executable)
{
	if (!Executable) return FExecutableHandle{};
	
	FExecutableContainer& Container = GetExecutableContainer(Owner);
	Container.GetExecutables().Add(Executable);
	return Executable->GenerateHandle();
}

void UCCORExecutor::RemoveExecutable( const UObject* Owner, const FExecutableHandle& Handle)
{
	if(ExecutableContainers.Contains(Owner))
	{
		FExecutableContainer& Container = ExecutableContainers[Owner];
		for (auto It = Container.GetExecutables().CreateIterator(); It; ++It)
		{
			if((*It)->GetID() == Handle.ID)
			{
				It.RemoveCurrent();
				return;
			}
		}
	}
}

void UCCORExecutor::Pause( const UObject* Owner )
{
	if( ExecutableContainers.Contains(Owner) )
	{
		ExecutableContainers[Owner].Pause();
	}
}

void UCCORExecutor::Start( const UObject* Owner )
{
	if(ExecutableContainers.Contains(Owner))
	{
		ExecutableContainers[Owner].Unpause();
	}
}

void UCCORExecutor::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	TickGlobal(DeltaTime);
	
	for (auto& Element : ExecutableContainers)
	{
		Element.Value.Tick(DeltaTime);
	}
}
