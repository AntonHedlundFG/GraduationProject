
#include "Grid/CGridLink.h"

UCGridLink::UCGridLink()
{
	
}

void UCGridLink::Initialize(ACGridTile* inStart, ACGridTile* inTarget)
{
	UE_LOG(LogTemp, Warning, TEXT("Link"));
	StartTile = inStart;
	TargetTile = inTarget;
}

