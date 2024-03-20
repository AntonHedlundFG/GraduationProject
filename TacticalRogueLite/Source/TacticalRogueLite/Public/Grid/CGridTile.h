
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGridTile.generated.h"

class ACGridContent;
class ACGrid;
class UCGridLink;

UCLASS()
class TACTICALROGUELITE_API ACGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridTile();

	UFUNCTION()
	void Initialize(ACGrid* inParentGrid, FVector2D inCoords);
	UFUNCTION()
	void CreateLinks();
	UFUNCTION()
	FVector2D GetGridCoords() const { return GridCoords; }
	UFUNCTION()
	TArray<UCGridLink*> GetStraightLinks() const { return StraightLinks; }
	UFUNCTION()
	TArray<UCGridLink*> GetDiagonalLinks() const { return DiagonalLinks; }
	UFUNCTION()
	TArray<ACGridTile*> GetNeighbours(bool bIncludeDiagonals = false);
	UFUNCTION()
	ACGridContent* GetContent() const { return TileContent; }
	UFUNCTION()
	void SetContent(ACGridContent* inContent) { TileContent = inContent; }


protected:
	UPROPERTY()
	TObjectPtr<ACGrid> ParentGrid;
	UPROPERTY()
	TArray<TObjectPtr<UCGridLink>> StraightLinks;
	UPROPERTY()
	TArray<TObjectPtr<UCGridLink>> DiagonalLinks;
	UPROPERTY()
	TObjectPtr<ACGridContent> TileContent;
	UPROPERTY()
	FVector2D GridCoords;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

};
