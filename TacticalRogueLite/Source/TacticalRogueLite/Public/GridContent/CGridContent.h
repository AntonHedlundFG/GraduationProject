
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

	ACGridTile* GetTile() const { return Tile; }
	void SetTile(ACGridTile* inTile) { Tile = inTile; }

protected:
	UPROPERTY()
	TObjectPtr<ACGridTile> Tile;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

};
