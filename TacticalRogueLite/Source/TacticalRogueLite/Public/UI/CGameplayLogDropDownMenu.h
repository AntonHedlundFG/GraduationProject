// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CDropDownWidget.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Logging/CLogManager.h"
#include "Components/CheckBox.h"
#include "CGameplayLogDropDownMenu.generated.h"

/**
 * 
 */
enum class EFilterCategory : int
{
	Gameplay = 1 << 0,  // Bit 0 represents Gameplay category
	Warning = 1 << 1,   // Bit 1 represents Warning category
	Error = 1 << 2,     // Bit 2 represents Error category
	Info = 1 << 3,      // Bit 3 represents Info category
	Network = 1 << 4    // Bit 4 represents Network category
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeEvent,int,Filter);
/*inline int GetFilterFlag(ELogCategory Category)
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
}*/
UCLASS()
class TACTICALROGUELITE_API UCGameplayLogDropDownMenu : public UCDropDownWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Gameplay_CheckBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Warning_CheckBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Error_CheckBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Info_CheckBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Network_CheckBox;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void UpdateFilter(bool bIsChecked);
	virtual void SetFilterFlag(EFilterCategory flag);
	virtual void RemoveFilterFlag(EFilterCategory flag);
public:
	UFUNCTION(BlueprintCallable)
	static int GetFilterFlag(ELogCategory Category);
	UFUNCTION(BlueprintCallable)
	int GetFilter(){return FilterFlag;}
	UPROPERTY(BlueprintAssignable)
	FOnChangeEvent OnFilterUpdated;
	int FilterFlag;
};
