
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
	UFUNCTION(BlueprintCallable)
	FVector2D GetGridCoords() const { return GridCoords; }
	UFUNCTION(BlueprintCallable)
	TArray<UCGridLink*> GetStraightLinks() const { return StraightLinks; }
	UFUNCTION(BlueprintCallable)
	TArray<UCGridLink*> GetDiagonalLinks() const { return DiagonalLinks; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetNeighbours(bool bIncludeDiagonals = false);
	UFUNCTION(BlueprintCallable)
	ACGridContent* GetContent() const { return TileContent; }
	UFUNCTION(BlueprintCallable)
	void SetContent(ACGridContent* inContent) { TileContent = inContent; }


protected:
	UPROPERTY(Replicated)
	TObjectPtr<ACGrid> ParentGrid;
	UPROPERTY(Replicated)
	TArray<TObjectPtr<UCGridLink>> StraightLinks;
	UPROPERTY(Replicated)
	TArray<TObjectPtr<UCGridLink>> DiagonalLinks;
	UPROPERTY(Replicated)
	TObjectPtr<ACGridContent> TileContent;
	UPROPERTY(Replicated)
	FVector2D GridCoords;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

};
