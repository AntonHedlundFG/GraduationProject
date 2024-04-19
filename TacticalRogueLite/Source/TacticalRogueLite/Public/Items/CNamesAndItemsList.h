#pragma once
#include "ItemData/CItemData.h"
#include "CNamesAndItemsList.generated.h"


USTRUCT(BlueprintType)
struct FCNamesAndItemsList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> Items;


	
	FCNamesAndItemsList& operator=(const FCNamesAndItemsList& Other)
	{
		Name = Other.Name;
		Items = Other.Items;
		
		return *this;
	}
	
};
