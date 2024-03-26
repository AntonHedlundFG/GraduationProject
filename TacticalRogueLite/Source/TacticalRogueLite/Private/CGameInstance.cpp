#include "CGameInstance.h"

#include "Utility/Logging/CLogManager.h"

void UCGameInstance::Init()
{
	Super::Init();
	UCLogManager::StartUp();
}

void UCGameInstance::Shutdown()
{
	UCLogManager::ShutDown();
	Super::Shutdown();
}
