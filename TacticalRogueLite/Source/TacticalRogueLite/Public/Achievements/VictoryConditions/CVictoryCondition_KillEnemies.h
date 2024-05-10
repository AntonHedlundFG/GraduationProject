
#pragma once

#include "CoreMinimal.h"
#include "Achievements/CVictoryCondition.h"
#include "CVictoryCondition_KillEnemies.generated.h"

class ACUnit;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCVictoryCondition_KillEnemies : public UCVictoryCondition
{
	GENERATED_BODY()

public:
	UCVictoryCondition_KillEnemies();
	
	virtual bool CheckVictoryCondition() override;

	UPROPERTY()
	TArray<ACUnit*> Enemies;


};
