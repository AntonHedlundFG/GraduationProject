#pragma once
#include "GridContent/UnitDataAssets/CUnitSpawnDetails.h"
#include "CEnemyType.generated.h"

class UCItemData;

USTRUCT(BlueprintType)
struct FCEnemyType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Details")
	int Level;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FCUnitSpawnDetails CharacterDetails;
};
