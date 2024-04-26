
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCharacterSelectMenuWidget.generated.h"

class UCSetControllingPlayerWidget;
class UCCharacterSelectorWidget;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCCharacterSelectMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	TArray<UCCharacterSelectorWidget*> CharacterSelectorsArray;
	TArray<UCSetControllingPlayerWidget*> PlayerSettersArray;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> ControllingPlayerIndex;
	UPROPERTY(BlueprintReadWrite)
	TArray<int> CharacterIndexes;
	UPROPERTY(BlueprintReadWrite)
	TArray<bool> LockedInfo;
	UPROPERTY(BlueprintReadWrite)
	TArray<bool> ReadyInfo;

	UFUNCTION(BlueprintCallable)
	void UpdateControllingPlayers(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateCharacters(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateLocks(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateReadyStatus(TArray<bool> inArray);
};
