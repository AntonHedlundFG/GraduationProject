#pragma once
#include "CStartCharacter.generated.h"

class UCItemData;

USTRUCT()
struct FCStartCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Details")
	FString CharacterType;

	UPROPERTY(EditAnywhere, Category = "Character Details")
	TArray<UCItemData*> ItemList;
	
};
