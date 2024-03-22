
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CTileHighlightData.generated.h"

enum class ETileHighlightModes : uint8;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCTileHighlightData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UMaterialInstance*> HighlightMaterials;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<ETileHighlightModes, UMaterialInstance*> HighlightMap;
	
};
