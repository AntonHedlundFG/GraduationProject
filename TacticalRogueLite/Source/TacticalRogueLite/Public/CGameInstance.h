#pragma once

#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

UCLASS()
class UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	
};
