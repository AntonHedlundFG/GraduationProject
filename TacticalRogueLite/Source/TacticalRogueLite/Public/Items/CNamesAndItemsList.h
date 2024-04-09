#pragma once
#include "CNamesAndItemsList.generated.h"

class UCItemData;

USTRUCT()
struct FCNamesAndItemsList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> Items;
};
