
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
	
};
