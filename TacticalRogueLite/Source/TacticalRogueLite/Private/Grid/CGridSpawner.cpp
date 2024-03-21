
#include "Grid/CGridSpawner.h"

#include "Grid/CGrid.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"

ACGridSpawner::ACGridSpawner()
{
	
}

void ACGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	ACGrid* grid = SpawnGrid(FVector::Zero(),10,10);
	SpawnUnitsFromArray(HeroUnits, grid->GetHeroSpawnTiles());
	SpawnUnitsFromArray(EnemyUnits, grid->GetEnemySpawnTiles());
}



TArray<ACUnit*> ACGridSpawner::SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles)
{
	TArray<ACUnit*> Units;
	for (int i = 0 ; i < inSpawnTiles.Num() ; i ++)
	{
		if (i >= inUnits.Num()) break;
		ACUnit* Unit = SpawnUnit(inUnits[i], inSpawnTiles[i]);
		Units.Add(Unit);
	}
	return Units;
}

ACUnit* ACGridSpawner::SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile)
{
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	SpawnPosition.Z += 100;
	TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActor<ACUnit>(inUnitType, SpawnPosition , FRotator::ZeroRotator);
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);
	return Unit;
}

ACGrid* ACGridSpawner::SpawnGrid(FVector inGridCenter, int inRows, int inColumns)
{
	ACGrid* SpawnedGrid = GetWorld()->SpawnActor<ACGrid>(Grid, inGridCenter, FRotator::ZeroRotator);
	SpawnedGrid->GenerateTiles(inRows, inColumns);

	return SpawnedGrid;
}



