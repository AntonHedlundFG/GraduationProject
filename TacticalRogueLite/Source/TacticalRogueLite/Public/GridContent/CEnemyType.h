#pragma once
#include "PaperSprite.h"
#include "CEnemyType.generated.h"

class UCItemData;

USTRUCT(BlueprintType)
struct FCEnemyType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Details")
	int Level;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TObjectPtr<UPaperSprite> Sprite;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> Items;
	
};
