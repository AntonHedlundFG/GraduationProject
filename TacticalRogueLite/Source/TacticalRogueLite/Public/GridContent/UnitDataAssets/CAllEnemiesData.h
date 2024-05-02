
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GridContent/UnitDataAssets/CEnemyType.h"
#include "CAllEnemiesData.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCAllEnemiesData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "All Enemies")
	TArray<FCEnemyType> EnemyLevelAndType;
	
};
