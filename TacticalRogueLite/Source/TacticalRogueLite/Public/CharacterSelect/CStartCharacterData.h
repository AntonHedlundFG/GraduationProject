
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Units/CClassSpecifics.h"
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
	TArray<UCClassSpecifics*> StartCharacterList;

	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	FString GetCharacterType(int index);
	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	UPaperSprite* GetCharacterSprite(int ClassIndex,int SpriteIndex);
	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	int GetSpriteListLength(int ClassIndex);
	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	TArray<UCItemData*> GetCharacterItems(int index);
	UFUNCTION(BlueprintCallable, Category = "Start Characters")
	FGameplayTag GetCharacterTag(int i);
	
};
