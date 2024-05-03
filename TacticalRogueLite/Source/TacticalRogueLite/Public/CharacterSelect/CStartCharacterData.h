
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "CStartCharacterData.generated.h"

struct FCUnitSpawnDetails;
class UCItemData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCStartCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Start Characters")
	TArray<FCUnitSpawnDetails> StartCharacterList;

	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	FString GetCharacterType(int index);
	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	TArray<UCItemData*> GetCharacterItems(int index);
	
};
