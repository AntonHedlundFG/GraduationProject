
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCharacterSelectorWidget.generated.h"

class ACCharacterSelectGameState;
class ACCharacterSelectGameController;
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
	
	UPROPERTY(BlueprintReadWrite, Category = "Starting Characters")
	int CharacterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Characters")
	TObjectPtr<UCStartCharacterData> StartCharacters;

	UFUNCTION(BlueprintCallable)
	void IncreaseIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseIndex();

	UFUNCTION(BlueprintPure)
	bool CanInteract();

protected:
	UPROPERTY()
	ACCharacterSelectGameState* GameState;
	UPROPERTY()
	ACCharacterSelectController* Controller;
	
};
