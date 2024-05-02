#pragma once
#include "CNamesAndItemsList.generated.h"

class UCItemData;
class UPaperSprite;

USTRUCT(BlueprintType)
struct FCNamesAndItemsList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TObjectPtr<UPaperSprite> Sprite;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> Items;


	
	// FCNamesAndItemsList& operator=(const FCNamesAndItemsList& Other)
	// {
	// 	Name = Other.Name;
	// 	Items = Other.Items;
	// 	
	// 	return *this;
	// }
	
};
