
#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileHighlightModes.h"
#include "GameFramework/Actor.h"
#include "GridContent/CGridContent.h"
#include "CGridTile.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightChange, bool, bIsHighlighted);


class UCTileHighlightData;
class ACGrid;

UCLASS()
class TACTICALROGUELITE_API ACGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGridTile();
	
	//Is managed by CPlayerController, with a bool for on/off.
	UPROPERTY(BlueprintAssignable)
	FOnHighlightChange OnHighlightChange;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Settings")
	TObjectPtr<UCTileHighlightData> HighlightData;
	
	

	UFUNCTION(BlueprintCallable)
	void Initialize(ACGrid* inParentGrid, FVector2D inCoords);
	UFUNCTION(BlueprintCallable)
	void GenerateLinks();

#pragma region Get/Set
	UFUNCTION(BlueprintCallable)
	ACGrid* GetParentGrid() const { return ParentGrid; }
	UFUNCTION(BlueprintCallable)
	FVector2D GetGridCoords() const { return GridCoords; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetStraightLinks() const { return StraightLinks; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetDiagonalLinks() const { return DiagonalLinks; }
	UFUNCTION(BlueprintCallable)
	TArray<ACGridTile*> GetNeighbours(bool bIncludeDiagonals = false);
	UFUNCTION(BlueprintCallable)
	ACGridContent* GetContent() const { return TileContent; }
	UFUNCTION(BlueprintCallable)
	void SetContent(ACGridContent* inContent) { TileContent = inContent; }
	UFUNCTION(BlueprintImplementableEvent)
	void SetTileHighlightMode(ETileHighlightModes inHighlightMode);
	UFUNCTION(BlueprintCallable)
	float GetCost() const { return TileContent ? TileContent->GetCost() : 0; }
#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Tile Info")
	TObjectPtr<ACGrid> ParentGrid;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Tile Info")
	TArray<TObjectPtr<ACGridTile>> StraightLinks;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Tile Info")
	TArray<TObjectPtr<ACGridTile>> DiagonalLinks;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Tile Info")
	TObjectPtr<ACGridContent> TileContent;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Tile Info")
	FVector2D GridCoords;

	ETileHighlightModes CurrentHighlightMode = ETileHighlightModes::ETHM_Default;

};
