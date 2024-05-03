
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCharacterSelectMenuWidget.generated.h"

class ACCharacterSelectGameState;
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Elements")
	TSubclassOf<UCCharacterSelectorWidget> CharacterSelectorBP;
	UPROPERTY(BlueprintReadWrite, Category = "UI Elements")
	TArray<UCCharacterSelectorWidget*> CharacterSelectorsArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Elements")
	TSubclassOf<UCSetControllingPlayerWidget> PlayerSetterBP;
	UPROPERTY(BlueprintReadWrite, Category = "UI Elements")
	TArray<UCSetControllingPlayerWidget*> PlayerSettersArray;

	UFUNCTION(BlueprintCallable)
	void UpdateAll();
	UFUNCTION(BlueprintCallable)
	void UpdateControllingPlayers(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateCharacters(TArray<int> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateLocks(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdateReadyStatus(TArray<bool> inArray);
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerCount(int inCount);

	UFUNCTION(BlueprintCallable)
	void CreateMenuWidgets();
	
	UFUNCTION(BlueprintCallable)
	void SetStateRef(ACCharacterSelectGameState* inStateRef) { StateRef = inStateRef; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACCharacterSelectGameState> StateRef;
};
