
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"
#include "Attributes/CAttributeComponent.h"
#include "Actions/CActionComponent.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Items/CInventoryComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

void ACUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACUnit, ControllingPlayerIndex);
	DOREPLIFETIME(ACUnit, UnitName);
}

ACUnit::ACUnit()
{
	bReplicates = true;
	
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));
	InventoryComp = CreateDefaultSubobject<UCInventoryComponent>(TEXT("InventoryComponent"));
	ActionComp = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
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

UCItemData* ACUnit::GetItemDataInSlot(FGameplayTag inSlot)
{
	return InventoryComp->GetItemInSlot(inSlot);
}

bool ACUnit::TryGetAbilityInSlot(FGameplayTag ItemSlot, FAbility& outAbility)
{
	return ActionComp->TryGetAbility(ItemSlot, outAbility);
}

TArray<FAbility> ACUnit::GetEquippedAbilities() const
{
	// TODO: There is probably a better way to do this
	TArray<FAbility> Abilities;
	const FGameplayTagContainer AbilitySlotsContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_ItemSlot);
	for (int i = 0; i < AbilitySlotsContainer.Num(); ++i)
	{
		FGameplayTag Slot = AbilitySlotsContainer.GetByIndex(i);
		FAbility Ability;
		if(ActionComp->TryGetAbility(Slot, Ability))
		{
			Abilities.Add(Ability);
		}
	}
	return Abilities;
}

bool ACUnit::HasRemainingCharges(FGameplayTag ItemSlot, int32 Amount)
{
	return AttributeComp->HasRemainingCharges(ItemSlot, Amount);
}

int32 ACUnit::GetRemainingCharges(FGameplayTag ItemSlot)
{
	return AttributeComp->GetRemainingCharges(ItemSlot);
}



