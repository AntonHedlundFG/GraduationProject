
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWinConWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALROGUELITE_API UCWinConWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateWinConText(FString inText);

protected:
	UPROPERTY(BlueprintReadWrite)
	FString CurrentText;
};
