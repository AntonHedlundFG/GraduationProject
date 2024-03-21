#pragma once
#include "CGridTile.h"

class UCItem;
class ACGridTile;
class CGridUtils
{
public:
	static TArray<FVector2D> StraightDirections();
	static TArray<FVector2D> DiagonalDirections();
	static TArray<ACGridTile*> BFS_Pathfinding(UCItem* inItem, ACGridTile* inStart, const ACGridTile* inTarget);
};


