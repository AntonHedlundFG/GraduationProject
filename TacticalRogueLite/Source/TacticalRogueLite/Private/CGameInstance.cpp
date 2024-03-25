#include "CGameInstance.h"

#include "Utility/Logging/CLogger.h"

void UCGameInstance::Init()
{
	Super::Init();
	UCLogger::StartUp();
}

void UCGameInstance::Shutdown()
{
	UCLogger::ShutDown();
	Super::Shutdown();
}
