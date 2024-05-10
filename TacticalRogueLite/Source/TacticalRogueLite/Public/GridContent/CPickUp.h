
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpriteMesh")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpriteMesh")
	TObjectPtr<UPaperSpriteComponent> SpriteComp;
	
	UPROPERTY()
	bool bIsPickedUp = false;
	
	virtual void HandleOnTileEnter(ACGridContent* inEnterContent) override;
	virtual void HandleOnTileExit(ACGridContent* inExitContent) override;

	UFUNCTION()
	virtual void GrantPickUpToUnit(ACUnit* inUnit) {}
};
