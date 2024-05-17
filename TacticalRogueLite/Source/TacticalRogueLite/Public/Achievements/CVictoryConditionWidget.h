
#pragma once

#include "CoreMinimal.h"
#include "UI/CResizableWindow.h"
#include "CVictoryConditionWidget.generated.h"

class UCLevelURLAsset;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCVictoryConditionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UButton> RestartButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainMenuRef;
	UPROPERTY(EditAnywhere, Category = "Level Loading")
	TObjectPtr<UCLevelURLAsset> LevelURLAsset;
	

	UFUNCTION(BlueprintCallable)
	virtual void SetUp();

	UFUNCTION(BlueprintCallable)
	void OpenWindow();
	
	UFUNCTION()
	void GoToMainMenu();
	UFUNCTION()
	void QuitGame();

	//Doesn't work great yet, will complete if there is time
	UFUNCTION()
	void Restart();
	
};
