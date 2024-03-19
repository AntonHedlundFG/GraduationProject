#pragma once

class CGridUtils
{
public:
	static TArray<FVector2D> StraightDirections()
	{
		TArray<FVector2D> Directions;
		Directions.Add(FVector2D(0,1));
		Directions.Add(FVector2D(0,-1));
		Directions.Add(FVector2D(-1,0));
		Directions.Add(FVector2D(1,0));
		
		return Directions;
	}

	static TArray<FVector2D> DiagonalDirections()
	{
		TArray<FVector2D> Directions;
		Directions.Add(FVector2D(1,1));
		Directions.Add(FVector2D(1,-1));
		Directions.Add(FVector2D(-1,1));
		Directions.Add(FVector2D(-1,-1));
		
		return Directions;
	}

};


