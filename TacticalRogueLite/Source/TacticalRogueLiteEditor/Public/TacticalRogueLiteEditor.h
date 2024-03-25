#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FTacticalRogueLiteEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};