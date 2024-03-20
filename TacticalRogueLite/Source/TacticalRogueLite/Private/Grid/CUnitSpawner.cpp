
#include "Grid/CUnitSpawner.h"

#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"

ACUnitSpawner::ACUnitSpawner()
{
	
}

void ACUnitSpawner::BeginPlay()
{
	Super::BeginPlay();
}



TArray<ACUnit*> ACUnitSpawner::SpawnUnitsFromArray(TArray<TSubclassOf<ACUnit>> inUnits, TArray<ACGridTile*> inSpawnTiles)
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

ACUnit* ACUnitSpawner::SpawnUnit(TSubclassOf<ACUnit> inUnitType, ACGridTile* inSpawnTile)
{
	FVector SpawnPosition = inSpawnTile->GetActorLocation();
	SpawnPosition.Z += 100;
	TObjectPtr<ACUnit> Unit = GetWorld()->SpawnActor<ACUnit>(inUnitType, SpawnPosition , FRotator::ZeroRotator);
	inSpawnTile->SetContent(Unit);
	Unit->SetTile(inSpawnTile);
	return Unit;
}



