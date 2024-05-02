#pragma once
#include "CUnitSpawnDetails.generated.h"

class UCItemData;
class UPaperSprite;

USTRUCT(BlueprintType)
struct FCUnitSpawnDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TObjectPtr<UPaperSprite> Sprite;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> Items;
};
