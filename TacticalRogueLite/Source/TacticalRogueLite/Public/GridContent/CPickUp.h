
#pragma once

#include "CoreMinimal.h"
#include "GridContent/CGridContent.h"
#include "CPickUp.generated.h"

class ACUnit;
class UPaperSpriteComponent;

UCLASS()
class TACTICALROGUELITE_API ACPickUp : public ACGridContent
{
	GENERATED_BODY()

public:
	ACPickUp();
	
	UFUNCTION()
	bool IsPickedUp() const { return bIsPickedUp; }
	
	UFUNCTION()
	virtual void RemoveFromBoard();
	UFUNCTION()
	virtual void ReturnToBoard(ACGridTile* inTile);
	UFUNCTION()
	virtual void GrantPickUpToUnit(ACUnit* inUnit) {}
	UFUNCTION()
	virtual void UndoGrantPickUpToUnit(ACUnit* inUnit) {}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpriteMesh")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpriteMesh")
	TObjectPtr<UPaperSpriteComponent> SpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Sprite, Category = "PickUp|SpriteMesh")
	TObjectPtr<UPaperSprite> Sprite;
	UPROPERTY()
	TObjectPtr<UPaperSprite> SavedSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Location, Category = "PickUp|WorldLocation")
	FVector Location;
	
	UPROPERTY(Replicated)
	bool bIsPickedUp = false;

	virtual void HandleOnTileEnter(ACGridContent* inEnterContent) override;
	virtual void HandleOnTileExit(ACGridContent* inExitContent) override;

	virtual void BeginPlay() override;


	
	UFUNCTION()
	void OnRep_Sprite();
	UFUNCTION()
	void OnRep_Location();
};
