
#include "Grid/CGridLink.h"

UCGridLink::UCGridLink()
{
	
}

void UCGridLink::Initialize(ACGridTile* inStart, ACGridTile* inTarget)
{
	StartTile = inStart;
	TargetTile = inTarget;
}

