
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

	int CurrentSelectedSpriteIndex = 0;
public:
	UPROPERTY(BlueprintReadWrite, Category = "Starting Characters")
	int WidgetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Characters")
	TObjectPtr<UCStartCharacterData> StartCharacters;

	UFUNCTION(BlueprintCallable)
	void IncreaseIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseIndex();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentCharacterVisuals(int inCharacterIndex, int SelectedSpriteIndex = 0);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateReadyVisuals(bool inIsReady);

	UFUNCTION(BlueprintCallable)
	void ToggleReady();

	UFUNCTION(BlueprintPure)
	bool CanInteract();

	UFUNCTION(BlueprintCallable)
	void StepSpriteIndex(int i);

protected:
	UPROPERTY()
	ACCharacterSelectGameState* GameState;
	
	UPROPERTY()
	ACCharacterSelectController* Controller;
	
};
