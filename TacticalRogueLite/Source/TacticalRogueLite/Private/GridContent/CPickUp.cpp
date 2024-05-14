
#include "GridContent/CPickUp.h"
#include "PaperSpriteComponent.h"
#include "Actions/CActionComponent.h"
#include "Grid/CGridTile.h"
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"

void ACPickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPickUp, Sprite);
	DOREPLIFETIME(ACPickUp, Location);
	DOREPLIFETIME(ACPickUp, bIsPickedUp);
	DOREPLIFETIME(ACPickUp, Tile);
}

ACPickUp::ACPickUp()
{
	bReplicates = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);
	Sprite = SpriteComp->GetSprite();
	OnRep_Sprite();
}

void ACPickUp::HandleOnTileEnter(ACGridContent* inEnterContent)
{
	Super::HandleOnTileEnter(inEnterContent);

	if (ACUnit* inUnit = Cast<ACUnit>(inEnterContent))
	{
		if (inUnit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Unit.IsEnemy"))))
			return;
			
		GrantPickUpToUnit(inUnit);
		RemoveFromBoard();
	}

}

void ACPickUp::HandleOnTileExit(ACGridContent* inExitContent)
{
	Super::HandleOnTileExit(inExitContent);

	if (ACUnit* outUnit = Cast<ACUnit>(inExitContent))
	{
		if (!bIsPickedUp &&
			outUnit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Unit.IsEnemy"))))
		{
			Tile->SetContent(this);
		}
	}
}

void ACPickUp::RemoveFromBoard()
{
	Tile = nullptr;
	bIsPickedUp = true;
		
	Sprite = nullptr;
	Location = FVector(0,0,-9999);
	OnRep_Sprite();
	OnRep_Location();
}

void ACPickUp::OnRep_Sprite()
{
	SpriteComp->SetSprite(Sprite);
}

void ACPickUp::OnRep_Location()
{
	SetActorLocation(Location);
}
