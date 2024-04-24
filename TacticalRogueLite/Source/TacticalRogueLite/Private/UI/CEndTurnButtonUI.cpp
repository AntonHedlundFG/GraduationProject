// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CEndTurnButtonUI.h"

#include "CGameMode.h"
#include "CPlayerController.h"
#include "Attributes/CAttributeComponent.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Kismet/GameplayStatics.h"



void UCEndTurnButtonUI::OnTurnUpdated()
{
	ACUnit* Unit;
	try
	{
		Unit = GameState->TurnOrder[0];	
	}
	catch (...)
	{
		return;
	}
	CurrentUnit = Unit;
	if(Unit == nullptr){return;}
	int ControllingPlayerIndex = Unit->ControllingPlayerIndex;
	bool bIsPlayer = ControllingPlayerIndex > 0;
	bool bIsOwning = Unit->IsControlledBy(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if(!bIsPlayer || !bIsOwning)
	{
		return;
	}

	UCAttributeComponent* AttributeComponent = Unit->GetAttributeComp();
	AttributeComponent->OnItemChargesChanged.AddUniqueDynamic(this,&UCEndTurnButtonUI::OnChargesChanged);
	UpdateItemCharges();

	if(ItemCharges == 0)
	{
		ShowEndButton();
	}
}

void UCEndTurnButtonUI::EndTurn()
{
	CurrentUnit->GetAttributeComp()->OnItemChargesChanged.RemoveDynamic(this,&UCEndTurnButtonUI::OnChargesChanged);
}

void UCEndTurnButtonUI::OnChargesChanged()
{
	LastUpdateItemCharges = ItemCharges;
	UpdateItemCharges();
	if(ItemCharges == 0 && LastUpdateItemCharges != 0)
	{
		ShowEndButton();
	}
	else if(ItemCharges > 0 && LastUpdateItemCharges == 0)
	{
		HideEndButton();
	}
}
void UCEndTurnButtonUI::ShowEndButton()
{
	if(IsAnimationPlaying(InAnimation) && !IsAnimationPlayingForward(InAnimation))
	{
		ReverseAnimation(InAnimation);
	}
	PlayAnimation(InAnimation);
}

void UCEndTurnButtonUI::HideEndButton()
{
	if(IsAnimationPlaying(InAnimation) && IsAnimationPlayingForward(InAnimation))
	{
		ReverseAnimation(InAnimation);
	}
	PlayAnimationReverse(InAnimation);
}

void UCEndTurnButtonUI::UpdateItemCharges()
{
	if(CurrentUnit == nullptr){return;}
	ItemCharges = 0;
	ItemCharges = CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Armor);
	ItemCharges = CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Trinket);
	ItemCharges = CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Boots);
	ItemCharges = CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Weapon);
}

void UCEndTurnButtonUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	GameState->OnTurnOrderUpdate.AddUniqueDynamic(this, &UCEndTurnButtonUI::OnTurnUpdated);
	EndButton->OnClicked.AddUniqueDynamic(this,&UCEndTurnButtonUI::EndTurn);
}
