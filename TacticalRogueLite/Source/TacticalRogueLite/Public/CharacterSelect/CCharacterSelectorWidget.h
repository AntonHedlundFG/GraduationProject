
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCharacterSelectorWidget.generated.h"

class ACCharacterSelectGameState;
class ACCharacterSelectController;
class UCStartCharacterData;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCCharacterSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Starting Characters")
	int WidgetIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Ready Status")
	bool bIsReadyToStart = false;

	UPROPERTY(BlueprintReadWrite, Category = "Starting Characters")
	int CharacterTypeIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Characters")
	TObjectPtr<UCStartCharacterData> StartCharacters;

	UFUNCTION(BlueprintCallable)
	void IncreaseIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseIndex();

	UFUNCTION(BlueprintCallable)
	void ToggleReady();

	UFUNCTION(BlueprintPure)
	bool CanInteract();

protected:
	UPROPERTY()
	ACCharacterSelectGameState* GameState;
	
	UPROPERTY()
	ACCharacterSelectController* Controller;
	
};
