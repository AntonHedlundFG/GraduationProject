
#include "GridContent/CPickUp.h"

#include "PaperSpriteComponent.h"
#include "GridContent/CUnit.h"

ACPickUp::ACPickUp()
{
	bReplicates = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);
}

void ACPickUp::HandleOnTileEnter(ACGridContent* inEnterContent)
{
	Super::HandleOnTileEnter(inEnterContent);

	if (ACUnit* inUnit = Cast<ACUnit>(inEnterContent))
	{
		Tile = nullptr;
		bIsPickedUp = true;
		GrantPickUpToUnit(inUnit);
		SetActorLocation(FVector(0,0,-9999));
		SpriteComp->SetSprite(nullptr);
	}

}

void ACPickUp::HandleOnTileExit(ACGridContent* inExitContent)
{
	Super::HandleOnTileExit(inExitContent);
}
