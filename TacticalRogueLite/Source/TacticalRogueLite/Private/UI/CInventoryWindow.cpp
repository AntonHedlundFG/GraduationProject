// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CInventoryWindow.h"

#include "Actions/CActionComponent.h"
#include "Attributes/CAttributeComponent.h"

FString UCInventoryWindow::GetClassAsString(ACUnit* Unit)
{
	UCAttributeComponent* AttributeComponent = Unit->GetAttributeComp();
	FGameplayTagContainer DebugContainer = Unit->GetGameplayTags();
	bool IsNecromancer = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Necromancer")));
	bool IsMage = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Mage")));
	bool IsMonk = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Monk")));
	bool IsWarrior = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Warrior")));
	bool IsCleric = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Cleric")));
	bool IsRanger = Unit->GetActionComp()->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Class.Ranger")));

	if(IsNecromancer)
	{
		return "Necromancer";
	}
	if(IsMage)
	{
		return "Mage";
	}
	if(IsMonk)
	{
		return "Monk";
	}
	if(IsWarrior)
	{
		return "Warrior";
	}
	if(IsCleric)
	{
		return "Cleric";
	}
	if(IsRanger)
	{
		return "Ranger";
	}
	return "";
}
