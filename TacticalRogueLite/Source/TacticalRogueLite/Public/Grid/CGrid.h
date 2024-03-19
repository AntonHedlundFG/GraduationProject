
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrid.generated.h"

class ACGridTile;

UCLASS()
class TACTICALROGUELITE_API ACGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<ACGridTile> TileBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int NodeInterval = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector2D GridDimensions = FVector2D(10,10);
	UPROPERTY()
	TArray<TObjectPtr<ACGridTile>> Tiles;
	UPROPERTY()
	TArray<FVector> TilesPositions;

	UFUNCTION()
	void GenerateGrid();
	UFUNCTION()
	ACGridTile* GetTileAtPosition(int inX, int inY);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector FindBottomLeftCorner() const;

};
