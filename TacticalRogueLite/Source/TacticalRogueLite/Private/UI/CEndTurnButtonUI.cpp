// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CEndTurnButtonUI.h"

#include "CGameMode.h"
#include "CPlayerController.h"
#include "Attributes/CAttributeComponent.h"
#include "GamePlayTags/SharedGamePlayTags.h"
#include "Kismet/GameplayStatics.h"



void UCEndTurnButtonUI::OnTurnUpdated()
{
	if(bIsOpen)
	{
		HideEndButton();
	}
	ACUnit* Unit;
	try
	{
		Unit = GameState->GetCurrentUnit();	
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
	if(!CurrentUnit){return;}
	CurrentUnit->GetAttributeComp()->OnItemChargesChanged.RemoveDynamic(this,&UCEndTurnButtonUI::OnChargesChanged);
	Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->EndTurn();
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
	bIsOpen = true;
	SetVisibility(ESlateVisibility::Visible);
	if(IsAnimationPlaying(FadeInAnimation) && !IsAnimationPlayingForward(FadeInAnimation))
	{
		ReverseAnimation(FadeInAnimation);
	}
	PlayAnimation(FadeInAnimation);
	ShowEndButton_BP();
}

void UCEndTurnButtonUI::HideEndButton()
{
	bIsOpen = false;
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if(IsAnimationPlaying(FadeInAnimation) && IsAnimationPlayingForward(FadeInAnimation))
	{
		ReverseAnimation(FadeInAnimation);
	}
	PlayAnimationReverse(FadeInAnimation);
	HideEndButton_BP();
}

void UCEndTurnButtonUI::UpdateItemCharges()
{
	if(CurrentUnit == nullptr){return;}
	ItemCharges = 0;
	ItemCharges += CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Armor);
	ItemCharges += CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Trinket);
	ItemCharges += CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Boots);
	ItemCharges += CurrentUnit->GetRemainingCharges(TAG_ItemSlot_Weapon);
}

void UCEndTurnButtonUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	GameState->OnTurnOrderUpdate.AddUniqueDynamic(this, &UCEndTurnButtonUI::OnTurnUpdated);
	EndButton->OnClicked.AddUniqueDynamic(this,&UCEndTurnButtonUI::EndTurn);
	HideEndButton();
}
