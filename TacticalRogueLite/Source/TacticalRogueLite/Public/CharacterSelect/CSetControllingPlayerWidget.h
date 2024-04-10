
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSetControllingPlayerWidget.generated.h"

class ACCharacterSelectController;
class UCCharacterSelectorWidget;
class ACCharacterSelectGameState;
class AOnlinePlayerState;
/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCSetControllingPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, Category = "Controlling Player")
	int WidgetIndex;
	
	UPROPERTY(BlueprintReadWrite, Category = "Controlling Player")
	bool bIsLocked = false;
	
	UFUNCTION(BlueprintCallable)
	void IncreaseIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseIndex();
	
protected:
	UPROPERTY()
	ACCharacterSelectGameState* GameState;
	UPROPERTY()
	ACCharacterSelectController* Controller;
};

