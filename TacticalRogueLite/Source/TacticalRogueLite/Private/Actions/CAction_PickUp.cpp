

#include "Actions/CAction_PickUp.h"

#include "GridContent/CPickUp.h"

void UCAction_PickUp::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);
	
	if (PickUp)
	{
		StartTile = PickUp->GetTile();
		PickUp->RemoveFromBoard();
		
		if (PickingUnit)
		{
			PickUp->GrantPickUpToUnit(PickingUnit);
		}
	}
}

void UCAction_PickUp::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);
}

void UCAction_PickUp::UndoAction(AActor* Instigator)
{
	Super::UndoAction(Instigator);

	if (PickUp && StartTile)
	{
		PickUp->ReturnToBoard(StartTile);

		if (PickingUnit)
		{
			PickUp->UndoGrantPickUpToUnit(PickingUnit);
		}
	}
}
