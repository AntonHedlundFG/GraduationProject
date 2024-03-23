
#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileHighlightModes.h"
#include "GameFramework/Actor.h"
#include "CGridTile.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightChange, bool, bIsHighlighted);


class UCTileHighlightData;
class ACGridContent;
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
	void CreateLinks();
	UFUNCTION(BlueprintCallable)

#pragma region Get/Set
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
#pragma endregion

protected:
	UPROPERTY(Replicated)
	TObjectPtr<ACGrid> ParentGrid;
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ACGridTile>> StraightLinks;
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ACGridTile>> DiagonalLinks;
	UPROPERTY(Replicated)
	TObjectPtr<ACGridContent> TileContent;
	UPROPERTY(Replicated)
	FVector2D GridCoords;

	ETileHighlightModes CurrentHighlightMode = ETileHighlightModes::ETHM_Default;
	
	virtual void BeginPlay() override;

};
