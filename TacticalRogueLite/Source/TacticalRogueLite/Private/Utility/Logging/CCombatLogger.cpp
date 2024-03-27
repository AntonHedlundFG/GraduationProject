// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Logging/CCombatLogger.h"
#include "Utility/Logging/CLogManager.h"

FString UCCombatLogger::Format(ECombatLogCategory Category, FString Message)
{
	FString StyleBegin = ToCombatLogStyleBegin(Category);
	return StyleBegin + Message + ((Category != ECombatLogCategory::DEFAULT)?StyleEnd: ""); 
}

FString UCCombatLogger::FormatIT(ECombatLogCategory Category, FString Instigator, FString Target, FString Message)
{
	FString StyleBegin = ToCombatLogStyleBegin(Category);
	return  StyleBegin + Instigator + Message + Target + ((Category != ECombatLogCategory::DEFAULT)?StyleEnd: ""); 
}

void UCCombatLogger::Log(FString Message, ECombatLogCategory Category = ECombatLogCategory::DEFAULT)
{
	const FString formattedText = Format(Category,Message);
	UCLogManager::Log(ELogCategory::LC_Gameplay,formattedText);
}
