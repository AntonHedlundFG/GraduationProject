
#include "GridContent/CUnit.h"

#include "CGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Attributes/CAttributeComponent.h"
#include "Actions/CActionComponent.h"
#include "Items/CInventoryComponent.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

void ACUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACUnit, ControllingPlayerIndex);


}

ACUnit::ACUnit()
{
	bReplicates = true;
	
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));
	InventoryComp = CreateDefaultSubobject<UCInventoryComponent>(TEXT("InventoryComponent"));
	ActionComp = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));
}

void ACUnit::BeginPlay()
{
	Super::BeginPlay();

	// if (GetNetMode() <= ENetMode::NM_ListenServer && IsValid(AttributeComp))
	// 	AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &ACUnit::OnHealthChanged);
	
}

void ACUnit::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	// if (GetNetMode() <= ENetMode::NM_ListenServer && IsValid(AttributeComp))
	// 	AttributeComp->OnHealthChanged.RemoveDynamic(this, &ACUnit::OnHealthChanged);
}

bool ACUnit::IsControlledBy(AController* inController)
{
	if (!inController->PlayerState)
		return ControllingPlayerIndex == 0; //AI controlled
	
	AOnlinePlayerState* PS = Cast<AOnlinePlayerState>(inController->PlayerState);
	if (!PS) return false;

	return PS->PlayerIndex == ControllingPlayerIndex;
}

<<<<<<< HEAD
UCItemData* ACUnit::GetItemInSlot(FGameplayTag inSlot)
{
	return InventoryComp->GetItemInSlot(inSlot);
=======
UCItemData* ACUnit::GetItemDataInSlot(FGameplayTag inSlot)
{
	return InventoryComp->GetItemInSlot(inSlot);
}

bool ACUnit::TryGetAbilityInSlot(FGameplayTag ItemSlot, FAbility& outAbility)
{
	return ActionComp->TryGetAbility(ItemSlot, outAbility);
>>>>>>> 9ed3bbc85c62c4c439be06b345fd147cf7f67a88
}


