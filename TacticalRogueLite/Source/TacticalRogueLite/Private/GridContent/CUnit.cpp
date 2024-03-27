
#include "GridContent/CUnit.h"

#include "CGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Items/ItemSlots.h"
#include "Attributes/CAttributeComponent.h"
#include "CommandPattern/CDeathConsequence.h"
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
	Inventory = CreateDefaultSubobject<UCInventoryComponent>(TEXT("InventoryComponent"));
}

void ACUnit::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() <= ENetMode::NM_ListenServer && IsValid(AttributeComp))
		AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &ACUnit::OnHealthChanged);
	
}

void ACUnit::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (GetNetMode() <= ENetMode::NM_ListenServer && IsValid(AttributeComp))
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &ACUnit::OnHealthChanged);
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
	return Inventory->GetItemInSlot(inSlot);
}

void ACUnit::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	float OldHealth = NewHealth - Delta;
	if (OldHealth > 0 && NewHealth <= 0)
	{
		ACGameMode* GameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
		if (GameMode)
		{
			UCDeathConsequence* Death = NewObject<UCDeathConsequence>(this);
			Death->DyingUnit = this;
			GameMode->RegisterAndExecuteConsequence(Death);
		}
	}
}
