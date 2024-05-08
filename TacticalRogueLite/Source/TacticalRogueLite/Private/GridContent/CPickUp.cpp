
#include "GridContent/CPickUp.h"

#include "GridContent/CUnit.h"

void ACPickUp::HandleOnTileEnter(ACGridContent* inEnterContent)
{
	Super::HandleOnTileEnter(inEnterContent);

	if (ACUnit* inUnit = Cast<ACUnit>(inEnterContent))
	{
		Tile = nullptr;
		bIsPickedUp = true;
	}
}

void ACPickUp::HandleOnTileExit(ACGridContent* inExitContent)
{
	Super::HandleOnTileExit(inExitContent);
}
