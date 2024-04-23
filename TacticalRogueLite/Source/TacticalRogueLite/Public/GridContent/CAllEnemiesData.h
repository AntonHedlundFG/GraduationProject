
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CAllEnemiesData.generated.h"

class ACUnit;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAllEnemiesData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "All Enemies")
	TMap<int, TSubclassOf<ACUnit>> EnemyLevelAndType;
	
};
