
#include "GridContent/CGridContent.h"
#include "Grid/CGridTile.h"
#include "Net/UnrealNetwork.h"

void ACGridContent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACGridContent, Tile);
}

ACGridContent::ACGridContent()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void ACGridContent::SetTile(ACGridTile* inTile)
{
	if (Tile)
	{
		Tile->SetContent(nullptr);
	}

	Tile = inTile;

	if (Tile)
	{
		Tile->SetContent(this);

		//Removed this line since we want to handle visual actor locations locally using UCActionVisualization
		//SetActorLocation(Tile->GetActorLocation() + FVector(0,0,100));
	}
}

void ACGridContent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACGridContent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

