// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CGameplayLogDropDownMenu.h"

void UCGameplayLogDropDownMenu::NativeConstruct()
{
	Super::NativeConstruct();
	Gameplay_CheckBox->OnCheckStateChanged.AddDynamic(this,&UCGameplayLogDropDownMenu::UpdateFilter);
	Warning_CheckBox->OnCheckStateChanged.AddDynamic(this,&UCGameplayLogDropDownMenu::UpdateFilter);
	Error_CheckBox->OnCheckStateChanged.AddDynamic(this,&UCGameplayLogDropDownMenu::UpdateFilter);
	Info_CheckBox->OnCheckStateChanged.AddDynamic(this,&UCGameplayLogDropDownMenu::UpdateFilter);
	Network_CheckBox->OnCheckStateChanged.AddDynamic(this,&UCGameplayLogDropDownMenu::UpdateFilter);
	Gameplay_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	SetFilterFlag(EFilterCategory::Gameplay);
}

void UCGameplayLogDropDownMenu::UpdateFilter(bool bIsChecked)
{
	if(Gameplay_CheckBox->IsChecked())
	{
		SetFilterFlag(EFilterCategory::Gameplay);
	}
	if(Warning_CheckBox->IsChecked())
	{
		SetFilterFlag(EFilterCategory::Warning);
	}
	if(Error_CheckBox->IsChecked())
	{
		SetFilterFlag(EFilterCategory::Error);
	}
	if(Info_CheckBox->IsChecked())
	{
		SetFilterFlag(EFilterCategory::Info);
	}
	if(Network_CheckBox->IsChecked())
	{
		SetFilterFlag(EFilterCategory::Network);
	}
	OnFilterUpdated.Broadcast(FilterFlag);
}

void UCGameplayLogDropDownMenu::SetFilterFlag(EFilterCategory flag)
{
	FilterFlag |= static_cast<int>(flag);
}

void UCGameplayLogDropDownMenu::RemoveFilterFlag(EFilterCategory flag)
{
	FilterFlag &= ~static_cast<int>(flag);
}

int UCGameplayLogDropDownMenu::GetFilterFlag(ELogCategory Category)
{
	int FilterFlag;
	switch (Category)
	{
	case ELogCategory::LC_Gameplay: FilterFlag = static_cast<int>(EFilterCategory::Gameplay); break;
	case ELogCategory::LC_Warning: FilterFlag = static_cast<int>(EFilterCategory::Warning);break;
	case ELogCategory::LC_Error:FilterFlag = static_cast<int>(EFilterCategory::Error); break;
	case ELogCategory::LC_Info: FilterFlag = static_cast<int>(EFilterCategory::Info);break;
	case ELogCategory::LC_Network: FilterFlag = static_cast<int>(EFilterCategory::Network);break;
	default: return 0;
	}
	return FilterFlag;
}
