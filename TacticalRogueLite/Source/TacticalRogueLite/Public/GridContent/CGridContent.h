
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "CGridContent.generated.h"

class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API ACGridContent : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridContent();

	UFUNCTION(BlueprintCallable, Category = "Grid|Tiles")
	ACGridTile* GetTile() const { return Tile; }

	//Note that this replaces the Content of the inTile. You have to manually handle this!
	UFUNCTION(BlueprintCallable, Category = "Grid|Tiles")
	void SetTile(ACGridTile* inTile);

	UFUNCTION(BlueprintCallable, Category = "Grid|Content")
	float GetCost() const { return Cost; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid|Content")
	FGameplayTagContainer GridContentTags;
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<ACGridTile> Tile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid|Content")
	float Cost = 0;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void HandleOnTileEnter(ACGridContent* inEnterContent) {}
	UFUNCTION()
	virtual void HandleOnTileExit(ACGridContent* inExitContent) {}

};
