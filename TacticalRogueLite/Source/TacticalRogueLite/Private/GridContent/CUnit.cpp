
#include "GridContent/CUnit.h"

#include "CGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Items/ItemSlots.h"
<<<<<<< HEAD
=======
#include "Attributes/CAttributeComponent.h"
>>>>>>> e07acfacf7ef898a0f36b8bf416ec0ed62ca572d
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

void ACUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACUnit, ControllingPlayerIndex);
}

ACUnit::ACUnit()
{
	bReplicates = true;
	//
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));
}

bool ACUnit::IsControlledBy(AController* inController)
{
	if (!inController->PlayerState)
		return ControllingPlayerIndex == 0; //AI controlled
	
	AOnlinePlayerState* PS = Cast<AOnlinePlayerState>(inController->PlayerState);
	if (!PS) return false;

	return PS->PlayerIndex == ControllingPlayerIndex;
}

UCItem* ACUnit::GetItemInSlot(EItemSlots inSlot)
{
	if (inSlot == EItemSlots::EIS_Boots)
		return TemporaryItemBoots;
	if (inSlot == EItemSlots::EIS_Weapon)
		return TemporaryItemWeapon;
	return nullptr;
}
