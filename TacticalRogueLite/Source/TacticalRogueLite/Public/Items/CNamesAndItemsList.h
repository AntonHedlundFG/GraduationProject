#pragma once
#include "CNamesAndItemsList.generated.h"

class UCItemData;

USTRUCT()
struct FCNamesAndItemsList
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UCItemData*> Items;
	UPROPERTY()
	FString Name;
};
