#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tiles/TileHighlightModes.h"
#include "CTileHighlightComponent.generated.h"

UCLASS()
class UCHighlightComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Init(UPrimitiveComponent* inPrimitiveComponent);
	UFUNCTION(BlueprintCallable)
	void AppendHighlightMode(ETileHighlightModes inHighlightMode);
	UFUNCTION(BlueprintCallable)
	void RemoveHighlightMode(ETileHighlightModes inHighlightMode);

private:
	UFUNCTION()
	void UpdateHighlight();
	
	UPROPERTY()
	TArray<ETileHighlightModes> HighlightModes;
	UPROPERTY()
	UPrimitiveComponent* HighlightedComponent;
	
};
