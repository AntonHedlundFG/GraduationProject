
#include "GridContent/CUnit.h"
#include "Net/UnrealNetwork.h"
#include "Attributes/CAttributeComponent.h"
#include "Actions/CActionComponent.h"
#include "AI/CAIController.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "UObject/ConstructorHelpers.h"
#include "Items/CInventoryComponent.h"
#include "PaperSpriteComponent.h"
#include "TacticalRogueLite/OnlineSystem/Public/OnlinePlayerState.h"

void ACUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACUnit, ControllingPlayerIndex);
	DOREPLIFETIME(ACUnit, UnitName);
	DOREPLIFETIME(ACUnit, TeamTag);
	DOREPLIFETIME(ACUnit, Sprite);

}

ACUnit::ACUnit()
{
	bReplicates = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);

	
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent")); //TODO: Old Attr Cleanup
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


void ACUnit::OnRep_Sprite()
{
	SpriteComp->SetSprite(Sprite);
}

bool ACUnit::IsControlledBy(AController* inController)
{
	if (!inController->PlayerState)
		return ControllingPlayerIndex == 0; //AI controlled
	
	AOnlinePlayerState* PS = Cast<AOnlinePlayerState>(inController->PlayerState);
	if (!PS) return false;

	return PS->PlayerIndex == ControllingPlayerIndex;
}

bool ACUnit::IsControlledLocally()
{
	auto* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return false;
	auto* PS = PC->GetPlayerState<AOnlinePlayerState>();
	if (!PS) return false;
	return ControllingPlayerIndex == PS->PlayerIndex;
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

void ACUnit::SetSprite(UPaperSprite* inSprite)
{
	Sprite = inSprite;
	OnRep_Sprite();
}

FGameplayTagContainer ACUnit::GetGameplayTags()
{
	return GridContentTags;
}

bool ACUnit::HasRemainingCharges(FGameplayTag ItemSlot, int32 Amount)
{
	return AttributeComp->HasRemainingCharges(ItemSlot, Amount);
}

int32 ACUnit::GetRemainingCharges(FGameplayTag ItemSlot)
{
	return AttributeComp->GetRemainingCharges(ItemSlot);
}



