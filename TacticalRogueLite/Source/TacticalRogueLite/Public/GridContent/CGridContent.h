
#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintCallable, Category = "Grid|Tiles")
	void SetTile(ACGridTile* inTile) { Tile = inTile; }

protected:
	TObjectPtr<ACGridTile> Tile;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

};
